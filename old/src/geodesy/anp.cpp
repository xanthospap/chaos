#include "geodesy.hpp"

/**
 * @details  Normalize an angle into the range 0 <= a < 2pi.
 *
 * @param[in] a the angle to normalize in radians (\c double)
 * @return      the normalized angle, in range [0,2pi) in radians (\c double).
 *
 */
double geo::anp(const double& a)
{
  double w (::fmod(a,D2PI));
  if (w<0) w += D2PI;
  return w;
}
