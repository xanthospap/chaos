#include "geodesy.hpp"
#include "gencon.hpp"

/**
 * @details This routine transforms from geocentric rectangular to 
 *          geodetic coordinates. This function is closely related to the GCONV2
 *          FORTRAN routine which can be found at 
 *          ftp://tai.bipm.org/iers/convupdt/chapter4/GCONV2.F
 *
 * @param[in]  a      equatorial Radius of the Earth in meters (\c double).
 * @param[in]  f      flattening form factor in meters (\c double).
 * @param[in]  x,y,z  rectangular coordinates in meters (\c double).
 * @param[out] phi    latitude coordinate on the ellipsoid in radians (\c double).
 * @param[out] lambda longitude coordinate on the ellipsoid in radians (\c double).
 * @param[out] h      height coordinate on the ellipsoid in meters (\c double).
 *
 * @note
 * - This routine is closely based on the GCONV2H subroutine by
 *   Toshio Fukushima
 * - There exist two more versions of this function, with different ellipsoid arguments.
**
** Test case:
**    given input: x = 4075579.496D0 meters  Wettzell (TIGO) station
**                 y =  931853.192D0 meters
**                 z = 4801569.002D0 meters
**
**    expected output: phi    =   0.857728298603D0 radians
**                     lambda =   0.224779294628D0 radians
**                     h      = 665.9207D0 meters
 *
 * \b references : \cite fukushima , \cite iers2010 .
 *
 * GCONV2 revision: 2010 September 2
 *
 * @todo perform the test case
 *
 */
void geo::car2ell (
  const double& a,const double& f,
  const double& x,const double& y, const double& z,
  double& phi,double& lambda,double& h)
{

/* Functions of ellipsoid parameters */
  const double aeps2 = a*a*1e-32;
  const double e2    = (2.0e0-f)*f;
  const double e4t   = e2*e2*1.5e0;
  const double ep2   = 1.0e0-e2;
  const double ep    = ::sqrt(ep2);
  const double aep   = a*ep;

/* ---------------------------------------------------------
* Compute Coefficients of (Modified) Quartic Equation
*
* Remark: Coefficients are rescaled by dividing by 'a'
* ---------------------------------------------------------*/

  /* Compute distance from polar axis squared */
  double p2 (x*x+y*y);

  /* Compute longitude lambda  */
  if (p2)
    lambda = ::atan2(y,x);
  else
    lambda=.0;

  /* Ensure that Z-coordinate is unsigned */
  double absz = fabs(z);

  /* Continue unless at the poles */
  if (p2>aeps2) {
    /* Compute distance from polar axis */
    double p = ::sqrt(p2);
    /* Normalize */
    double s0 = absz/a;
    double pn = p/a;
    double zp = ep*s0;
    /* Prepare Newton correction factors. */
    double c0  = ep*pn;
    double c02 = c0*c0;
    double c03 = c02*c0;
    double s02 = s0*s0;
    double s03 = s02*s0;
    double a02 = c02+s02;
    double a0  = ::sqrt(a02);
    double a03 = a02*a0;
    double d0  = zp*a03 + e2*s03;
    double f0  = pn*a03 - e2*c03;
    /* Prepare Halley correction factor. */
    double b0 = e4t*s02*c02*pn*(a0-ep);
    double s1 = d0*f0 - b0*s0;
    double cp = ep*(f0*f0-b0*c0);
    /* Evaluate latitude and height. */
    phi = ::atan(s1/cp);
    double s12 = s1*s1;
    double cp2 = cp*cp;
    h = (p*cp+absz*s1-a*::sqrt(ep2*s12+cp2))/::sqrt(s12+cp2);
  } else {
  /* Special case: pole. */
    phi = DPI/2e0;
    h   = absz - aep;
  }

  /* Restore sign of latitude. */
  if (z<0.)
    phi = -phi;

/* Finished. */
  return;
}

/**
 * @details This routine transforms from geocentric rectangular to 
 *          geodetic coordinates. This function is closely related to the GCONV2
 *          FORTRAN routine which can be found at 
 *          ftp://tai.bipm.org/iers/convupdt/chapter4/GCONV2.F
 *
 * @param[in]  ell    the ellipsoid to use (\c geo::ellipsoid).
 * @param[in]  x,y,z  rectangular coordinates in meters (\c double).
 * @param[out] phi    latitude coordinate on the ellipsoid in radians (\c double).
 * @param[out] lambda longitude coordinate on the ellipsoid in radians (\c double).
 * @param[out] h      height coordinate on the ellipsoid in meters (\c double).
 *
 * @note
 * - This routine is closely based on the GCONV2H subroutine by
 *   Toshio Fukushima
 * - There exist two more versions of this function, with different ellipsoid arguments.
 *
 * \b references : \cite fukushima , \cite iers2010 .
 *
 * GCONV2 revision: 2010 September 2
 *
 */
void geo::car2ell (const Ellipsoid& ell,
  const double& x,const double& y, const double& z,
  double& phi,double& lambda,double& h)
{
  const double a = ell.semimajor();
  const double f = ell.flattening();
  car2ell (a,f,x,y,z,phi,lambda,h);
  return;
}

/**
 * @details This routine transforms from geocentric rectangular to 
 *          geodetic coordinates. This function is closely related to the GCONV2
 *          FORTRAN routine which can be found at 
 *          ftp://tai.bipm.org/iers/convupdt/chapter4/GCONV2.F
 *
 * @param[in]  x,y,z  rectangular coordinates in meters (\c double).
 * @param[out] phi    latitude coordinate on the ellipsoid in radians (\c double).
 * @param[out] lambda longitude coordinate on the ellipsoid in radians (\c double).
 * @param[out] h      height coordinate on the ellipsoid in meters (\c double).
 *
 * @note
 * - This routine is closely based on the GCONV2H subroutine by
 *   Toshio Fukushima
 * - This version uses the default ellipsoid, which is grs80.
 * - There exist two more versions of this function, with different ellipsoid arguments.
 *
 * \b references : \cite fukushima , \cite iers2010 .
 *
 * GCONV2 revision: 2010 September 2
 *
 */
void geo::car2ell(
  const double& x,const double& y, const double& z,
  double& phi,double& lambda,double& h)
{
  Ellipsoid ell ("GRS80");
  const double a = ell.semimajor();
  const double f = ell.flattening();
  car2ell (a,f,x,y,z,phi,lambda,h);
  return;
}
