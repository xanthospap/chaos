#include "datetime.hpp"

/**
 * @details Decompose days to hours, minutes, seconds, fraction. This function 
 *          is closely related to the SOFA function iauD2tf.
 *
 * <B>SOFA lib</B> <br>
 * This revision:  2013 June 18 <br>
 * SOFA release 2013-12-02
 *
 * @param[in]      ndp    Resolution (Note 1) (\c integer).
 * @param[in]      days   Interval in days (\c double).
 * @param[out]     sign   '+' or '-' (\c char).
 * @param[out]     ihmsf  Hours, minutes, seconds, fraction (\c int[4]).
 *
 * @note
 * -# The argument \p ndp is interpreted as follows:
 *    ndp   |     resolution
 *    ------|---------------
 *     :    |  ...0000 00 00
 *    -7    |     1000 00 00
 *    -6    |      100 00 00
 *    -5    |       10 00 00
 *    -4    |        1 00 00
 *    -3    |        0 10 00
 *    -2    |        0 01 00
 *    -1    |        0 00 10
 *     0    |        0 00 01
 *     1    |        0 00 00.1
 *     2    |        0 00 00.01
 *     3    |        0 00 00.001
 *     :    |        0 00 00.000...
 *
 * -# The largest positive useful value for \p ndp is determined by the
 *    size of days, the format of double on the target platform, and
 *    the risk of overflowing \p ihmsf[3].  On a typical platform, for
 *    days up to 1.0, the available floating-point precision might
 *    correspond to ndp=12. <b>However, the practical limit is typically
 *    ndp=9, set by the capacity of a 32-bit int, or ndp=4 if int is
 *    only 16 bits.</b>
 * -# The absolute value of days may exceed 1.0.  In cases where it
 *    does not, it is up to the caller to test for and handle the
 *    case where days is very nearly 1.0 and rounds up to 24 hours,
 *    by testing for \p ihmsf[0] = 24 and setting \p ihmsf[0-3] to zero.
*/
void datetime::d2tf(const int& ndp,const double& days, 
  char* sign,int ihmsf[4])
{

  int nrs, n;
  double rs, rm, rh, a, w, ah, am, as, af;


  /* Handle sign. */
  *sign = (char) ( ( days >= 0.0 ) ? '+' : '-' );

  /* Interval in seconds. */
  a = DAYSEC * fabs (days);

  /* Pre-round if resolution coarser than 1s (then pretend ndp=1). */
  if (ndp < 0) {
    nrs = 1;
    for (n = 1; n <= -ndp; n++) {
      nrs *= (n == 2 || n == 4) ? 6 : 10;
    }
    rs = (double) nrs;
    w = a / rs;
    a = rs * std::round(w);
  }

  /* Express the unit of each field in resolution units. */
  nrs = 1;
  for (n = 1; n <= ndp; n++) {
    nrs *= 10;
  }
  rs = (double) nrs;
  rm = rs * 60.0;
  rh = rm * 60.0;

  /* Round the interval and express in resolution units. */
  a = std::round (rs * a);

  /* Break into fields. */
  ah = a / rh;
  ah = dint(ah);
  a -= ah * rh;
  am = a / rm;
  am = dint(am);
  a -= am * rm;
  as = a / rs;
  as = dint(as);
  af = a - as * rs;

  /* Return results. */
  ihmsf[0] = (int) ah;
  ihmsf[1] = (int) am;
  ihmsf[2] = (int) as;
  ihmsf[3] = (int) af;

  return;
}
