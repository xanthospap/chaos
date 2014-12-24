#include "datetime.hpp"

/**
 * @details Julian Date to Gregorian Calendar, expressed in a form convenient
 * for formatting messages:  rounded to a specified precision. This function is
 * closely related to the SOFA function iauJdcalf.
 *
 * <B>SOFA lib</B> <br>
 * This revision: 2013 June 18 <br>
 * SOFA release 2013-12-02
 *
 * @param[in]  ndp       Number of decimal places of days in fraction (\c integer).
 * @param[in]  dj1,dj2   Julian Date, i.e. dj1+dj2 = Julian Date (Note 1) 
 *                       (\c double).
 * @param[out] iymdf     Year, month, day, fraction in Gregorian calendar 
 *                       (\c integer[4]).
 * @return               An \c integer denoting the exit status
 *
 * Returned Int  | Status
 * ------------- | -------------
 * -1            | Date out of range
 *  0            | OK
 * +1            | \p ndp not 0-9 (interpreted as 0)
 *
 * @note
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
 * -# In early eras the conversion is from the "Proleptic Gregorian
 *    Calendar";  no account is taken of the date(s) of adoption of
 *    the Gregorian Calendar, nor is the AD/BC numbering convention
 *    observed.
 * -# Refer to the function jd2cal.
 * -# \p ndp should be 4 or less if internal overflows are to be
 *    avoided on machines which use 16-bit integers.
 *
 * \b Reference: \cite esaa , Section 12.92 (p604).
 *
 */
int datetime::jdcalf(const int& ndp,const double& dj1,const double& dj2,
  int iymdf[4])
{

  int j, js;
  double denom, d1, d2, f1, f2, f;

  /* Denominator of fraction (e.g. 100 for 2 decimal places). */
  if ((ndp >= 0) && (ndp <= 9)) {
    j = 0;
    denom = pow(10.0e0, ndp);
  } else {
    j = 1;
    denom = 1.0e0;
  }

  /* Copy the date, big then small, and realign to midnight. */
  if (dj1 >= dj2) {
    d1 = dj1;
    d2 = dj2;
  } else {
    d1 = dj2;
    d2 = dj1;
  }
  d2 -= 0.5e0;

  /* Separate days and fractions. */
  f1 = fmod(d1, 1.0e0);
  f2 = fmod(d2, 1.0e0);
  d1 = floor(d1 - f1);
  d2 = floor(d2 - f2);

  /* Round the total fraction to the specified number of places. */
  f = floor((f1+f2)*denom + 0.5e0) / denom;

  /* Re-assemble the rounded date and re-align to noon. */
  d2 += f + 0.5e0;

  /* Convert to Gregorian calendar. */
  js = datetime::jd2cal (d1,d2,iymdf[0],iymdf[1],iymdf[2],f);
  if (js == 0) {
    iymdf[3] = (int) (f * denom);
  } else {
    j = js;
  }

  /* Return the status. */
  return j;
}
