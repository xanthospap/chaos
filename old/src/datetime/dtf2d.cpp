#include "datetime.hpp"
#include <string.h>

/**
 * @details Encode date and time fields into 2-part Modified Julian Date (or in the case
 * of UTC a quasi-JD form that includes special provision for leap
 * seconds). This function is closely related to the SOFA function iaudtf2d.
 *
 * <B>SOFA lib</B> <br>
 * This revision:  2013 July 26 <br>
 * SOFA release 2013-12-02
 *
 * @param[in] scale     Time scale ID (Note 1) (\c char[]).
 * @param[in] iy,im,id  Year, month, day in Gregorian calendar (Note 2) (\c integer).
 * @param[in] ihr,imn   Hour, minute (\c integer).
 * @param[in] sec       Seconds (\c double).
 * @param[out] d1,d2    2-part Modified Julian Date (\c double).
 * @return              An \c integer denoting the exit status
 *
 * Returned Int  | Status
 * ------------- | -------------
 * +3            | Both of next two
 * +2            | Time is after end of day (Note 5)
 * +1            | Dubious year (Note 6)
 *  0            | OK
 * -1            | Bad year
 * -2            | Bad month
 * -3            | Bad day
 * -4            | Bad hour
 * -5            | Bad minute
 * -6            | Bad second (<0)
 *
 * @attention This version will arrange \c d1 and \c d2 so that the first is an
 *            integer (i.e. Modified Julian Day) and the second is the decimal
 *            part (i.e. fractional part of day). The original SOFA (which returns JD
 *            not MJD) routine is available as dtf2d_.
 *
 * @note
 * -# \p scale identifies the time scale.  Only the value "UTC" (in upper
 *    case) is significant, and enables handling of leap seconds
 * -# For calendar conventions and limitations, see cal2jd.
 * -# The sum of the results, \p d1 + \p d2, is Modified Julian Date, where \p d1
 *    is the Modified Julian Day and \p d2 is the fraction of a day.  In the
 *    case of UTC, where the use of MJD is problematical, special
 *    conventions apply:  see the next note.
 * -# MJD cannot unambiguously represent UTC during a leap second unless
 *    special measures are taken.  The SOFA internal convention is that
 *    the quasi-MJD day represents UTC days whether the length is 86399,
 *    86400 or 86401 SI seconds.  In the 1960-1972 era there were
 *    smaller jumps (in either direction) each time the linear UTC(TAI)
 *    expression was changed, and these "mini-leaps" are also included
 *    in the SOFA convention.
 * -# The warning status "time is after end of day" usually means that
 *    the sec argument is greater than 60.0.  However, in a day ending
 *    in a leap second the limit changes to 61.0 (or 59.0 in the case
 *    of a negative leap second).
 * -# The warning status "dubious year" flags UTCs that predate the
 *    introduction of the time scale or that are too far in the future
 *    to be trusted.  See iauDat for further details.
 * -# Only in the case of continuous and regular time scales (TAI, TT,
 *    TCG, TCB and TDB) is the result \p d1 + \p d2 a Julian Date, strictly
 *    speaking.  In the other cases (UT1 and UTC) the result must be
 *    used with circumspection;  in particular the difference between
 *    two such results cannot be interpreted as a precise time
 *    interval.
 *
 * @see datetime::dtf2d_
 *
 */
int datetime::dtf2d(
  const char *scale,const int& iy,const int& im,const int& id,
  const int& ihr,const int& imn,const double& sec,
  double& d1,double& d2)
{

  int js, iy2, im2, id2;
  double dj, w, day, seclim, dat0, dat12, dat24, dleap, time;


  /* Today's Modified Julian Day Number. */
  js = datetime::cal2jd (iy,im,id,dj,w);
  if ( js ) return js;
  /*dj += w;*/

  /* Day length and final minute length in seconds (provisional). */
  day = DAYSEC;
  seclim = 60.0e0;

  /* Deal with the UTC leap second case. */
  if ( ! strcmp(scale,"UTC") ) {

    /* TAI-UTC at 0h today. */
    js = datetime::dat (iy,im,id,0.0,dat0);
    if ( js < 0 ) return js;

    /* TAI-UTC at 12h today (to detect drift). */
    js = datetime::dat (iy,im,id,0.5,dat12);
    if ( js < 0 ) return js;

    /* TAI-UTC at 0h tomorrow (to detect jumps). */
    js = datetime::jd2cal (dj+DJM0,1.5,iy2,im2,id2,w); // different from SOFA
    if ( js ) return js;
    js = datetime::dat (iy2,im2,id2,0.0,dat24);
    if ( js < 0 ) return js;

    /* Any sudden change in TAI-UTC between today and tomorrow. */
    dleap = dat24 - (2.0e0*dat12 - dat0);

    /* If leap second day, correct the day and final minute lengths. */
    day += dleap;
    if ( ihr == 23 && imn == 59 ) seclim += dleap;

    /* End of UTC-specific actions. */
  }

  /* Validate the time. */
  if ( ihr >= 0 && ihr <= 23 ) {
    if ( imn >= 0 && imn <= 59 ) {
      if ( sec >= 0 ) {
        if ( sec >= seclim ) {
          js += 2;
        }
      } else {
        js = -6;
      }
    } else {
      js = -5;
    }
  } else {
    js = -4;
  }
  if ( js < 0 ) return js;

  /* The time in days. */
  time  = ( 60.0e0 * ( (double) ( 60 * ihr + imn ) ) + sec ) / day;

  /* Return the date and time. */
  d1 = dj;
  d2 = time;

  /* Status. */
  return js;
}

/**
 * @details Encode date and time fields into 2-part Julian Date (or in the case
 * of UTC a quasi-JD form that includes special provision for leap
 * seconds). This function is closely related to the SOFA function iaudtf2d.
 *
 * <B>SOFA lib</B> <br>
 * This revision:  2013 July 26 <br>
 * SOFA release 2013-12-02
 *
 * @param[in] scale     Time scale ID (Note 1) (\c char[]).
 * @param[in] iy,im,id  Year, month, day in Gregorian calendar (Note 2) (\c integer).
 * @param[in] ihr,imn   Hour, minute (\c integer).
 * @param[in] sec       Seconds (\c double).
 * @param[out] d1,d2    2-part Julian Date (Notes 3,4) (\c double).
 * @return              An \c integer denoting the exit status
 *
 * Returned Int  | Status
 * ------------- | -------------
 * +3            | Both of next two
 * +2            | Time is after end of day (Note 5)
 * +1            | Dubious year (Note 6)
 *  0            | OK
 * -1            | Bad year
 * -2            | Bad month
 * -3            | Bad day
 * -4            | Bad hour
 * -5            | Bad minute
 * -6            | Bad second (<0)
 *
 * @note
 * -# \p scale identifies the time scale.  Only the value "UTC" (in upper
 *    case) is significant, and enables handling of leap seconds (see
 *    Note 4).
 * -# For calendar conventions and limitations, see cal2jd.
 * -# The sum of the results, \p d1 + \p d2, is Julian Date, where normally \p d1
 *    is the Julian Day Number and \p d2 is the fraction of a day.  In the
 *    case of UTC, where the use of JD is problematical, special
 *    conventions apply:  see the next note.
 * -# JD cannot unambiguously represent UTC during a leap second unless
 *    special measures are taken.  The SOFA internal convention is that
 *    the quasi-JD day represents UTC days whether the length is 86399,
 *    86400 or 86401 SI seconds.  In the 1960-1972 era there were
 *    smaller jumps (in either direction) each time the linear UTC(TAI)
 *    expression was changed, and these "mini-leaps" are also included
 *    in the SOFA convention.
 * -# The warning status "time is after end of day" usually means that
 *    the sec argument is greater than 60.0.  However, in a day ending
 *    in a leap second the limit changes to 61.0 (or 59.0 in the case
 *    of a negative leap second).
 * -# The warning status "dubious year" flags UTCs that predate the
 *    introduction of the time scale or that are too far in the future
 *    to be trusted.  See iauDat for further details.
 * -# Only in the case of continuous and regular time scales (TAI, TT,
 *    TCG, TCB and TDB) is the result \p d1 + \p d2 a Julian Date, strictly
 *    speaking.  In the other cases (UT1 and UTC) the result must be
 *    used with circumspection;  in particular the difference between
 *    two such results cannot be interpreted as a precise time
 *    interval.
 */
int datetime::dtf2d_(
  const char *scale,const int& iy,const int& im,const int& id,
  const int& ihr,const int& imn,const double& sec,
  double& d1,double& d2)
{
  int js, iy2, im2, id2;
  double dj, w, day, seclim, dat0, dat12, dat24, dleap, time;


  /* Today's Julian Day Number. */
  js = datetime::cal2jd_ (iy,im,id,dj,w);
  if ( js ) return js;
  dj += w;

  /* Day length and final minute length in seconds (provisional). */
  day = DAYSEC;
  seclim = 60.0e0;

  /* Deal with the UTC leap second case. */
  if ( ! strcmp(scale,"UTC") ) {

    /* TAI-UTC at 0h today. */
    js = datetime::dat (iy,im,id,0.0,dat0);
    if ( js < 0 ) return js;

    /* TAI-UTC at 12h today (to detect drift). */
    js = datetime::dat (iy,im,id,0.5,dat12);
    if ( js < 0 ) return js;

    /* TAI-UTC at 0h tomorrow (to detect jumps). */
    js = datetime::jd2cal (dj,1.5,iy2,im2,id2,w);
    if ( js ) return js;
    js = datetime::dat (iy2,im2,id2,0.0,dat24);
    if ( js < 0 ) return js;

    /* Any sudden change in TAI-UTC between today and tomorrow. */
    dleap = dat24 - (2.0e0*dat12 - dat0);

    /* If leap second day, correct the day and final minute lengths. */
    day += dleap;
    if ( ihr == 23 && imn == 59 ) seclim += dleap;

    /* End of UTC-specific actions. */
  }

  /* Validate the time. */
  if ( ihr >= 0 && ihr <= 23 ) {
    if ( imn >= 0 && imn <= 59 ) {
      if ( sec >= 0 ) {
        if ( sec >= seclim ) {
          js += 2;
        }
      } else {
        js = -6;
      }
    } else {
      js = -5;
    }
  } else {
    js = -4;
  }
  if ( js < 0 ) return js;

  /* The time in days. */
  time  = ( 60.0e0 * ( (double) ( 60 * ihr + imn ) ) + sec ) / day;

  /* Return the date and time. */
  d1 = dj;
  d2 = time;

  /* Status. */
  return js;
}
