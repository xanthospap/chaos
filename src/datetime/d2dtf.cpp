#include "datetime.hpp"
#include <string.h>

/**
 * @details Format for output a 2-part Julian Date (or in the case of UTC a
 *          quasi-JD form that includes special provision for leap seconds). 
 *          This function is closely related to the SOFA function iauD2dtf.
 * 
 * <B>SOFA lib</B> <br>
 * This revision:  2013 July 26 <br>
 * SOFA release 2013-12-02
 *
 * @param[in]      scale Time scale ID (Note 1) (\c char[]).
 * @param[in]      ndp   Resolution (Note 2) (\c integer).
 * @param[in]      d1    First part of a 2-part Julian Date (Notes 3,4) 
 *                       (\c double).
 * @param[in]      d2    Second part of a 2-part Julian Date (Notes 3,4) 
 *                       (\c double).
 * @param[out]     iy    Year (\c integer).
 * @param[out]     im    Month (\c integer).
 * @param[out]     id    Day (\c integer).
 * @param[out]     ihmsf Hours, minutes, seconds, fraction (Note 1)
 *                       (\c int[4]).
 * @return               An \c integer denoting the exit status
 *
 * Returned Int  | Status
 * ------------- | -------------
 * +1            | Dubious year (Note 5)
 *  0            | OK
 * -1            | Unacceptable date (Note 6)
 *
 * @note
 * -# \p scale identifies the time scale.  Only the value "UTC" (in upper
 *    case) is significant, and enables handling of leap seconds (see
 *    Note 4).
 * -# \p ndp is the number of decimal places in the seconds field, and can
 *    have negative as well as positive values, such as:
 *    ndp | resolution
 *    ----|-----------
 *    -4  | 1 00 00
 *    -3  | 0 10 00
 *    -2  | 0 01 00
 *    -1  | 0 00 10
 *     0  | 0 00 01
 *     1  | 0 00 00.1
 *     2  | 0 00 00.01
 *     3  | 0 00 00.001
 *    <b>The limits are platform dependent, but a safe range is -5 to +9.</b>
 * -# \p d1 + \p d2 is Julian Date, apportioned in any convenient way between
 *    the two arguments, for example where d1 is the Julian Day Number
 *    and d2 is the fraction of a day.  In the case of UTC, where the
 *    use of JD is problematical, special conventions apply:  see the
 *    next note.
 * -# JD cannot unambiguously represent UTC during a leap second unless
 *    special measures are taken.  The SOFA internal convention is that
 *    the quasi-JD day represents UTC days whether the length is 86399,
 *    86400 or 86401 SI seconds.  In the 1960-1972 era there were
 *    smaller jumps (in either direction) each time the linear UTC(TAI)
 *    expression was changed, and these "mini-leaps" are also included
 *    in the SOFA convention.
 * -# The warning status "dubious year" flags UTCs that predate the
 *    introduction of the time scale or that are too far in the future
 *    to be trusted.  See iauDat for further details.
 * -# For calendar conventions and limitations, see iauCal2jd.
 *
 */
int datetime::d2dtf(
  const char *scale,const int& ndp,const double& d1,const double& d2,
  int& iy,int& im,int& id,int ihmsf[4])
{

  int leap;
  char s;
  int iy1, im1, id1, js, iy2, im2, id2, ihmsf1[4], i;
  double /*a1, b1,*/ fd, dat0, dat12, w, dat24, dleap;

  /* Provisional calendar date. */
  js = datetime::jd2cal (d1,d2,iy1,im1,id1,fd);
  if ( js ) return -1;

  /* Is this a leap second day? */
  leap = 0;
  if (!strcmp(scale,"UTC") ) {

    /* TAI-UTC at 0h today. */
    js = datetime::dat (iy1,im1,id1,0.0,dat0);
    if ( js < 0 ) return -1;

    /* TAI-UTC at 12h today (to detect drift). */
    js = datetime::dat (iy1,im1,id1,0.5,dat12);
    if ( js < 0 ) return -1;

    /* TAI-UTC at 0h tomorrow (to detect jumps). */
    js = datetime::jd2cal (d1+1.5, d2-fd,iy2,im2,id2,w);
    if ( js ) return -1;
    js = datetime::dat (iy2,im2,id2,0.0,dat24);
    if ( js < 0 ) return -1;

    /* Any sudden change in TAI-UTC (seconds). */
    dleap = dat24 - (2.0*dat12 - dat0);

    /* If leap second day, scale the fraction of a day into SI. */
    leap = (dleap != 0.0);
    if (leap) fd += fd * dleap/DAYSEC;

   /* End of UTC-specific actions. */
  }

  /* Provisional time of day. */
  datetime::d2tf (ndp,fd,&s,ihmsf1);

  /* Is this a leap second day? */
  if (!leap ) {

    /* No.  Has the time rounded up to 24h? */
    if ( ihmsf1[0] > 23 ) {

      /* Yes.  We will need tomorrow's calendar date. */
      js = datetime::jd2cal (d1+1.5,d2-fd,iy2,im2,id2,w);

      /* Use 0h tomorrow. */
      iy1 = iy2;
      im1 = im2;
      id1 = id2;
      for ( i = 0; i < 4; i++ ) ihmsf1[i] = 0;
    }

  } else {

    /* This is a leap second day.  Has the time reached or passed 24h? */
    if ( ihmsf1[0] > 23 ) {

      /* Yes.  Use 23 59 60... */
      ihmsf1[0] = 23;
      ihmsf1[1] = 59;
      ihmsf1[2] = 60;
    }
  }

  /* Results. */
  iy = iy1;
  im = im1;
  id = id1;
  for ( i = 0; i < 4; i++ ) ihmsf[i] = ihmsf1[i];

  /* Status. */
  return js < 0 ? -1 : js;
}
