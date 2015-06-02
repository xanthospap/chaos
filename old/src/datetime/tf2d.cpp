#include "datetime.hpp"

/**
 * @details Convert hours, minutes, seconds to days. This function is
 * closely related to the SOFA function iauTf2d.
 *
 * <B>SOFA lib</B> <br>
 * This revision: 2013 June 18 <br>
 * SOFA release 2013-12-02
 *
 * @param[in]  s       Sign:  '-' = negative, otherwise positive (\c char).
 * @param[in]  ihour   Hours (\c integer).
 * @param[in]  imin    Minutes (\c integer).
 * @param[in]  sec     Seconds (\c double).
 * @param[out] days    Interval in days (\c double).
 * @return             An \c integer denoting the exit status
 *
 * Returned Int  | Status
 * ------------- | -------------
 *  0            | OK
 * +1            | \p ihour outside range 0-23
 * +2            | \p imin outside range 0-59
 * +3            | \p sec outside range 0-59.999...
 *
 * @note
 * -#  The result is computed even if any of the range checks fail.
 * -#  Negative \p ihour, \p imin and/or \p sec produce a warning status, but
 *     the absolute value is used in the conversion.
 * -#  If there are multiple errors, the status value reflects only the
 *     first, the smallest taking precedence.
 *
 */
int datetime::tf2d(char s,const int& ihour,const int& imin,const double& sec,
  double& days)
{

  /* Compute the interval. */
  days  = ( s == '-' ? -1.0 : 1.0 ) *
          ( 60.0 * ( 60.0 * ( (double) std::abs (ihour) ) +
                            ( (double) std::abs (imin) ) ) +
                            fabs (sec) ) / DAYSEC;

  /* Validate arguments and return status. */
  if ( ihour < 0 || ihour > 23 ) return 1;
  if ( imin < 0 || imin > 59 ) return 2;
  if ( sec < 0.0 || sec >= 60.0 ) return 3;
  
  /* All ok; return */
  return 0;
}
