#include "geodesy.hpp"

/**
 * @details This subroutine transforms the vector Xij (=Xj-Xi), from cartesian to
 *          the local level system (N,E,U), given ellispoid parameters (a,f).
 *          Reference point is Pi. All units are in meters.
 *
 * @param[in]  a        equatorial Radius of the Earth in meters (\c double).
 * @param[in]  f        flattening form factor in meters (\c double).
 * @param[in]  xi,yi,zi rectangular coordinates of reference point in meters (\c double).
 * @param[in]  xj,yj,zj rectangular coordinates of second point in meters (\c double).
 * @param[out] north    north component in local level system in meters (\c double).
 * @param[out] east     east component in local level system in meters (\c double).
 * @param[out] up       up component in local level system in meters (\c double).
 *
 * @note  There exist two more versions of this function, with different ellipsoid arguments.
 *
 *  * \b reference: \cite pg Chapter 5.9
 *
 */
void geo::car2top(
  const double& a,const double& f,
  const double& xi,const double& yi, const double& zi,
  const double& xj,const double& yj, const double& zj,
  double& north, double& east, double& up)
{

  /* ellipsoidal coordinates of reference point */
  double phi_i,lambda_i,h_i;
  
  /* cartesian to ellipsoidal for reference point */
  car2ell (a,f,xi,yi,zi,phi_i,lambda_i,h_i);

  /* trigonometric numbers */
  double cosf (::cos(phi_i));
  double cosl (::cos(lambda_i));
  double sinf (::sin(phi_i));
  double sinl (::sin(lambda_i));

  /* catresian vector */
  double dx = xj - xi;
  double dy = yj - yi;
  double dz = zj - zi;

  /* topocentric vector */
  north = - sinf * cosl * dx - sinf * sinl * dy + cosf * dz;
  east  = - sinl * dx        + cosl * dy;
  up    =   cosf * cosl * dx + cosf * sinl * dy + sinf * dz;

  /* finished */
  return; 
}

/**
 * @details This subroutine transforms the vector Xij (=Xj-Xi), from cartesian to
 *          the local level system (N,E,U), given ellispoid parameters (a,f).
 *          Reference point is Pi. All units are in meters.
 *
 * @param[in]  ell      the ellipsoid to use (\c geo::ellipsoid).
 * @param[in]  xi,yi,zi rectangular coordinates of reference point in meters (\c double).
 * @param[in]  xj,yj,zj rectangular coordinates of second point in meters (\c double).
 * @param[out] north    north component in local level system in meters (\c double).
 * @param[out] east     east component in local level system in meters (\c double).
 * @param[out] up       up component in local level system in meters (\c double).
 *
 * @note  There exist two more versions of this function, with different ellipsoid arguments.
 *
 */
void geo::car2top(
  const Ellipsoid& ell,
  const double& xi,const double& yi, const double& zi,
  const double& xj,const double& yj, const double& zj,
  double& north, double& east, double& up)
{
  const double a = ell.semimajor();
  const double f = ell.flattening();
  car2top (a,f,xi,yi,zi,xj,yj,zj,north,east,up);
  return;
}

/**
 * @details This subroutine transforms the vector Xij (=Xj-Xi), from cartesian to
 *          the local level system (N,E,U), given ellispoid parameters (a,f).
 *          Reference point is Pi. All units are in meters.
 *
 * @param[in]  xi,yi,zi rectangular coordinates of reference point in meters (\c double).
 * @param[in]  xj,yj,zj rectangular coordinates of second point in meters (\c double).
 * @param[out] north    north component in local level system in meters (\c double).
 * @param[out] east     east component in local level system in meters (\c double).
 * @param[out] up       up component in local level system in meters (\c double).
 *
 * @note 
 *   - There exist two more versions of this function, with different ellipsoid arguments.
 *   - This version uses the default ellipsoid, which is grs80 (@see geo::ellipsoid()).
 *
 */
void geo::car2top(
  const double& xi,const double& yi, const double& zi,
  const double& xj,const double& yj, const double& zj,
  double& north, double& east, double& up)
{
  Ellipsoid ell ("GRS80");
  const double a = ell.semimajor();
  const double f = ell.flattening();
  car2top (a,f,xi,yi,zi,xj,yj,zj,north,east,up);
  return;
}
