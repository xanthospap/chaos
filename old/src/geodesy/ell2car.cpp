#include "geodesy.hpp"

/**
 * @details This subroutine transforms from geodetic (i.e. latitude, longtitude,
 *          height on the ellipsoid) to  geocentric rectangular coordinates. 
 *          Units are meters and radians.
 *
 * @param[in]  a      equatorial Radius of the Earth in meters (\c double).
 * @param[in]  f      flattening form factor in meters (\c double).
 * @param[in]  phi    latitude coordinate on the ellipsoid in radians (\c double).
 * @param[in]  lambda longitude coordinate on the ellipsoid in radians (\c double).
 * @param[in]  h      height coordinate on the ellipsoid in meters (\c double).
 * @param[out] x,y,z  rectangular coordinates in meters (\c double).
 *
 * @note there exist two more versions of this function, with different ellipsoid arguments.
 *
 * @todo provide test case
 *
 */
void geo::ell2car (
  const double& a,const double& f,
  const double& phi,const double& lambda,const double& h,
  double& x, double& y, double& z)
{

  /* Functions of ellipsoid parameters */
  const double e2 = (2.0e0-f)*f;

  /* Trigonometric numbers */
  double sinf (::sin(phi));
  double cosf (::cos(phi));
  double sinl (::sin(lambda));
  double cosl (::cos(lambda));

  /* radius of curvature in the prime vertical */
  const double N = a / ::sqrt(1.0e0-e2*sinf*sinf);

  /* compute geocentric rectangular coordinates */
  x = (N+h) * cosf * cosl;
  y = (N+h) * cosf * sinl;
  z = ((1.0e0-e2) * N + h) * sinf;

  /* Finished. */
  return;
}

/**
 * @details This subroutine transforms from geodetic (i.e. latitude, longtitude,
 *          height on the ellipsoid) to  geocentric rectangular coordinates. 
 *          Units are meters and radians.
 *
 * @param[in]  ell    the ellipsoid to use (\c geo::ellipsoid).
 * @param[in]  phi    latitude coordinate on the ellipsoid in radians (\c double).
 * @param[in]  lambda longitude coordinate on the ellipsoid in radians (\c double).
 * @param[in]  h      height coordinate on the ellipsoid in meters (\c double).
 * @param[out] x,y,z  rectangular coordinates in meters (\c double).
 *
 * @note there exist two more versions of this function, with different ellipsoid arguments.
 *
 */
void geo::ell2car (
  const Ellipsoid& ell,
  const double& phi,const double& lambda,const double& h,
  double& x, double& y, double& z)
{
  const double a = ell.semimajor();
  const double f = ell.flattening();
  ell2car (a,f,phi,lambda,h,x,y,z);
  return;
}

/**
 * @details This subroutine transforms from geodetic (i.e. latitude, longtitude,
 *          height on the ellipsoid) to  geocentric rectangular coordinates. 
 *          Units are meters and radians.
 *
 * @param[in]  phi    latitude coordinate on the ellipsoid in radians (\c double).
 * @param[in]  lambda longitude coordinate on the ellipsoid in radians (\c double).
 * @param[in]  h      height coordinate on the ellipsoid in meters (\c double).
 * @param[out] x,y,z  rectangular coordinates in meters (\c double).
 *
 * @note 
 *   - This version uses the default ellipsoid, which is grs80.
 *   - there exist two more versions of this function, with different ellipsoid arguments.
 *
 */
void geo::ell2car (
  const double& phi,const double& lambda,const double& h,
  double& x, double& y, double& z)
{
  Ellipsoid ell ("GRS80");
  const double a = ell.semimajor();
  const double f = ell.flattening();
  ell2car (a,f,phi,lambda,h,x,y,z);
  return;
}
