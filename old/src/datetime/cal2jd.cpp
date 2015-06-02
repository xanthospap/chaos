#include "datetime.hpp"
#include <algorithm>

/**
 * @details Gregorian Calendar to Modified  Julian Date. This function is closely related
 *          to the SOFA function iauCal2jd. 
 *
 * <B>SOFA lib</B> <br>
 * This revision:  2013 August 7 <br>
 * SOFA release 2013-12-02
 *
 * @param[in]     iy   Year (\c integer).
 * @param[in]     im   Month (\c integer).
 * @param[in]     id   Day (\c integer).
 * @param[out]    djm0 First of a 2-part MJD. This is always an integer, 
 *                     i.e. the Modified Julian Day (\c double).
 * @param[out]    djm  Second of a 2-part MJD. This is always .0, i.e.
 *                     the fraction of day (\c double).
 * @return             An \c integer denoting the exit status
 *
 * Returned Int  | Status
 * ------------- | -------------
 *  0            | OK
 * -1            | Bad year (ΜJD not computed)
 * -2            | Bad month (ΜJD not computed)
 * -3            | Bad day (ΜJD computed)
 *
 * @note
 *  -# The algorithm used is valid from -4800 March 1, but this
 *     implementation rejects dates before -4799 January 1.
 *  -# The Modified Julian Date is returned in two pieces MJ Day and 0,
 *     which is designed to preserve time resolution. The
 *     Modified Julian Date is available as a single number by adding djm0 and
 *     djm.
 *  -# The original SOFA routine, returned the JD and arranged the terms 
 *     \c djm0 and \c djm, so that:
 *     - \c djm0 MJD zero-point: always 2400000.5
 *     - \c djm Modified Julian Date for 0 hrs
 *     <br>In this version, the resulting MJD (<b>and not JD</b>) is rearanged 
 *     so that the first part is always an integer (i.e. the Modified Julian Day),
 *     while the second is always 0.0 (the decimal part of day).
 *     For example, the date 2014/10/13 will be resolved as:
 *     Routine | djm0              | djm
 *     --------|-------------------|---------
 *     cal2jd_ |2400000.50 (=DJM0) |56943.00
 *     cal2jd  |  56943.00         |    0.0
 *     Note that the original SOFA routine is preserved as cal2jd_.
 *
 * @see cal2jd_
 *
 * \b Reference: \cite esaa , Section 12.92 (p604).
 *
 */
int datetime::cal2jd(const int& iy,const int& im,const int& id,
  double& djm0,double& djm)
{

  /* Earliest year allowed (4800BC) */
  const int IYMIN = -4799;

  /* Month lengths in days */
  static const int mtab[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  /* Preset status. */
  int j = 0;

  /* Validate year and month. */
  if (iy < IYMIN) return -1;
  if (im < 1 || im > 12) return -2;

  /* If February in a leap year, 1, otherwise 0. */
  int ly = ((im == 2) && !(iy%4) && (iy%100 || !(iy%400)));

  /* Validate day, taking into account leap years. */
  if ( (id < 1) || (id > (mtab[im-1] + ly))) j = -3;

  /* Return result. */
  int my = (im - 14) / 12;
  long iypmy = (long) (iy + my);
  djm0 = 0.e0;
  djm = (double)((1461L * (iypmy + 4800L)) / 4L
    + (367L * (long) (im - 2 - 12 * my)) / 12L
    - (3L * ((iypmy + 4900L) / 100L)) / 4L
    + (long) id - 2432076L);

  /* at this point, djm0 is MJD zero-point: always 2400000.5 and 
  djm is the Modified Julian Date for 0 hrs. Rearange the terms, so that
  the first is an integer and the second is the fraction */
  std::swap (djm0,djm);

  /* Return status. */
  return j;
}

/**
 * @details Gregorian Calendar to Julian Date. This function is closely related
 *          to the SOFA function iauCal2jd. 
 *
 * <B>SOFA lib</B> <br>
 * This revision:  2013 August 7 <br>
 * SOFA release 2013-12-02
 *
 * @param[in]     iy   Year (\c integer).
 * @param[in]     im   Month (\c integer).
 * @param[in]     id   Day (\c integer).
 * @param[out]    djm0 MJD zero-point: always 2400000.5 (\c double).
 * @param[out]    djm  Modified Julian Date for 0 hrs (\c double).
 * @return             An \c integer denoting the exit status
 *
 * Returned Int  | Status
 * ------------- | -------------
 *  0            | OK
 * -1            | Bad year (Note 3: JD not computed)
 * -2            | Bad month (JD not computed)
 * -3            | Bad day (JD computed)
 *
 * @note
 *  -# The algorithm used is valid from -4800 March 1, but this
 *     implementation rejects dates before -4799 January 1.
 *  -# The Julian Date is returned in two pieces, in the usual SOFA
 *     manner, which is designed to preserve time resolution.  The
 *     Julian Date is available as a single number by adding djm0 and
 *     djm.
 *  -# In early eras the conversion is from the "Proleptic Gregorian
 *     Calendar";  no account is taken of the date(s) of adoption of
 *     the Gregorian Calendar, nor is the AD/BC numbering convention
 *     observed.
 *
 * \b Reference: \cite esaa , Section 12.92 (p604).
 *
 */
int datetime::cal2jd_ (const int& iy,const int& im,const int& id,
  double& djm0,double& djm)
{

  /* Earliest year allowed (4800BC) */
  const int IYMIN = -4799;

  /* Month lengths in days */
  static const int mtab[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  /* Preset status. */
  int j = 0;

  /* Validate year and month. */
  if (iy < IYMIN) return -1;
  if (im < 1 || im > 12) return -2;

  /* If February in a leap year, 1, otherwise 0. */
  int ly = ((im == 2) && !(iy%4) && (iy%100 || !(iy%400)));

  /* Validate day, taking into account leap years. */
  if ( (id < 1) || (id > (mtab[im-1] + ly))) j = -3;

  /* Return result. */
  int my = (im - 14) / 12;
  long iypmy = (long) (iy + my);
  djm0 = DJM0;
  djm = (double)((1461L * (iypmy + 4800L)) / 4L
    + (367L * (long) (im - 2 - 12 * my)) / 12L
    - (3L * ((iypmy + 4900L) / 100L)) / 4L
    + (long) id - 2432076L);

  /* Return status. */
  return j;
}
