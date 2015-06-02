#include "geodesy.hpp"

/**
 * @details This subroutine computes for a topocentric vector (N,E,U)
 *          the distance, azimouth angle and zenith distance.
 *
 * @param[in]  north    north component in local level system in meters (\c double).
 * @param[in]  east     east component in local level system in meters 
 *                      (\c double).
 * @param[in]  up       up component in local level system in meters (\c double).
 * @param[out] distance spatial length of vector in meters (\c double).
 * @param[out] azimouth azimouth in range [0-2pi) in radians (\c double).
 * @param[out] zenith   zenith distance clockwise from up to vector in radians 
 *                      (\c double).
 * @return              an integer denoting the status of the computation 
 *                      (\c integer). The integer can be:
 * Return value | Status
 * -------------|-------------------------------------
 *    0         | all ok
 *    1         | distance is zero (zero division); only distance computed
 *    2         | north component is zero (zero division); only distance computed
 *
 * \b reference: \cite pg Chapter 5.9
 *
 */
int geo::topsaz(
  const double& north,const double& east,const double& up,
  double& distance, double& azimouth, double& zenith)
{

  /* spatial distance of vector */
  distance  = ::sqrt(north*north + east*east + up*up);

  /* check if zero distance or north are zero */
  if ((!distance)||(!north)) {
    if (!distance) {
        return 1;
    }
    else {
        return 2;
    }
  }

  /* azimouth */
  azimouth = ::atan2 (east,north);

  /* normalize to range [0-2pi) */
  azimouth = geo::anp (azimouth);

  /* zenith angle */
  zenith = ::acos (up/distance);

  /* finished */
  return 0;
}
