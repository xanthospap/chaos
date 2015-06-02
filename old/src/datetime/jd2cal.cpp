#include "datetime.hpp"

/**
 * @details Julian Date to Gregorian year, month, day, and fraction of a day.
 * This function is closely related to the SOFA function iaujd2cal.
 *
 * <B>SOFA lib</B> <br>
 * This revision: 2013 August 7 <br>
 * SOFA release 2013-12-02
 *
 * @param[in]  dj1,dj2   Julian Date (Notes 1, 2) (\c double).
 * @param[out] iy,im,id  Year, month, day (\c integer).
 * @param[out] fd        Fraction of day (\c double)
 * @return               An \c integer denoting the exit status
 *
 * Returned Int  | Status
 * ------------- | -------------
 *  0            | OK
 * -1            | Unacceptable date (Note 3)
 *
 * @note
 * -# The earliest valid date is -68569.5 (-4900 March 1).  The
 *    largest value accepted is 1e9.
 * -# The Julian Date is apportioned in any convenient way between
 *    the arguments \p dj1 and \p dj2.  For example, JD=2450123.7 could
 *    be expressed in any of these ways, among others:
 *    \p dj1      |     \p dj2   |  Method
 *    ------------|--------------|--------------------
 *    2450123.7   |        0.0   |    JD method
 *    2451545.0   |    -1421.3   |    J2000 method
 *    2400000.5   |    50123.2   |    MJD method
 *    2450123.5   |        0.2   |    date & time method
 *
 * -# In early eras the conversion is from the "proleptic Gregorian
 *    calendar";  no account is taken of the date(s) of adoption of
 *    the Gregorian calendar, nor is the AD/BC numbering convention
 *    observed.
 * 
 * \b Reference: \cite esaa , Section 12.92 (p604).
 *
 */
int datetime::jd2cal(const double& dj1,const double& dj2,
  int& iy, int& im,int& id,double& fd)
{

  /* Minimum and maximum allowed JD */
  const double DJMIN = -68569.5;
  const double DJMAX = 1e9;

  long jd, l, n, i, k;
  double dj, d1, d2, f1, f2, f, d;


  /* Verify date is acceptable. */
  dj = dj1 + dj2;
  if (dj < DJMIN || dj > DJMAX) return -1;

  /* Copy the date, big then small, and re-align to midnight. */
  if (dj1 >= dj2) {
    d1 = dj1;
    d2 = dj2;
  } else {
    d1 = dj2;
    d2 = dj1;
  }
  d2 -= 0.5e0;

  /* Separate day and fraction. */
  f1 = fmod(d1, 1.0e0);
  f2 = fmod(d2, 1.0e0);
  f = fmod(f1 + f2, 1.0e0);
  if (f < 0.0) f += 1.0e0;
  d = floor(d1 - f1) + floor(d2 - f2) + floor(f1 + f2 - f);
  jd = (long) floor(d) + 1L;

  /* Express day in Gregorian calendar. */
  l = jd + 68569L;
  n = (4L * l) / 146097L;
  l -= (146097L * n + 3L) / 4L;
  i = (4000L * (l + 1L)) / 1461001L;
  l -= (1461L * i) / 4L - 31L;
  k = (80L * l) / 2447L;
  id = (int) (l - (2447L * k) / 80L);
  l = k / 11L;
  im = (int) (k + 2L - 12L * l);
  iy = (int) (100L * (n - 49L) + i + l);
  fd = f;

  return 0;
}
