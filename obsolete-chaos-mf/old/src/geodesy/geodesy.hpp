#ifndef _GEODESY_HPP
#define _GEODESY_HPP

/**
 * @file      geodesy.hpp
 * @version   1.0-1b
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 * @date      October, 2014
 *
 * @brief     Functions of geodetic interest.
 *
 * @details   This file contains a list of functions to perform usual geodetic
 *            operations and transformations.
 *
 * @todo 
 *
 * @copyright No COPYRIGHT whatsoever.
 *
 * <b><center><hr>
 * National Technical University of Athens <br>
 *      Dionysos Satellite Observatory     <br>
 *        Higher Geodesy Laboratory        <br>
 *      http://dionysos.survey.ntua.gr
 * </center></b>
 */

#include <cmath>
#include "datums.hpp"
#include "gencon.hpp"

namespace geo {

/*! Normalize angle into the range 0 <= a < 2pi. */
double anp(const double& a);

/*! @brief Transform cartesian coordinates to ellipsoidal given an ellipsoid. 
 *         Units are meters/radians. 
 */
void car2ell(const Ellipsoid& ell,
  const double& x,const double& y, const double& z,
  double& phi,double& lambda,double& h);

/*! @brief Transform cartesian coordinates to ellipsoidal given semi-major axis 
 *         and flattening. Units are meters/radians. 
 */
void car2ell(const double& a,const double& f,
  const double& x,const double& y, const double& z,
  double& phi,double& lambda,double& h);

/*! @brief Transform cartesian coordinates to ellipsoidal. Ellipsoid used is the
 *         default (GRS80). Units are meters/radians. 
 */
void car2ell(
  const double& x,const double& y, const double& z,
  double& phi,double& lambda,double& h);

/*! @brief Transform ellipsoidal to cartesian coordinates given an ellipsoid 
 *         object. Units are meters/radians. 
 */
void ell2car(
  const Ellipsoid& ell,
  const double& phi,const double& lambda,const double& h,
  double& x, double& y, double& z);

/*! @brief Transform ellipsoidal to cartesian coordinates given semi-major axis 
 *         and flattening. Units are meters/radians. 
 */
void ell2car(const double& a,const double& f,
  const double& phi,const double& lambda,const double& h,
  double& x, double& y, double& z);

/*! @brief Transform ellipsoidal coordinates to cartesian. Ellipsoid used is the
 *         default (GRS80). Units are meters/radians. 
 */
void ell2car(
  const double& phi,const double& lambda,const double& h,
  double& x, double& y, double& z);

/*! @brief Transform the vector <tt>Xij (=Xj-Xi)</tt>, from cartesian to the 
 *         local level system \c (N,E,U), given an ellispoid. Reference point
 *         is \c Pi. 
 */
void car2top(
  const Ellipsoid& e,
  const double& xi,const double& yi, const double& zi,
  const double& xj,const double& yj, const double& zj,
  double& north, double& east, double& up);

/*! @brief Transform the vector <tt>Xij (=Xj-Xi)</tt>, from cartesian to the 
 *         local level system \c (N,E,U), given semi-major axis and flattening. 
 *         Reference point is \c Pi. 
 */
void car2top(
  const double& a,const double& f,
  const double& xi,const double& yi, const double& zi,
  const double& xj,const double& yj, const double& zj,
  double& north, double& east, double& up);

/*! @brief Transform the vector <tt>Xij (=Xj-Xi)</tt>, from cartesian to the 
 *         local level system \c (N,E,U). Reference point is \c Pi. Ellipsoid 
 *         used is the default (GRS80). 
 */
void car2top(
  const double& xi,const double& yi, const double& zi,
  const double& xj,const double& yj, const double& zj,
  double& north, double& east, double& up);

/*! @brief Compute for a topocentric vector \c (N,E,U) the distance, azimouth 
 *         angle and zenith distance. Units are meters/radians. 
 */
int topsaz(
  const double& north,const double& east,const double& up,
  double& distance, double& azimouth, double& zenith);

}; /* end of geo namespace */

#endif
