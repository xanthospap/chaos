#ifndef _INTERPOLATION_HPP
#define _INTERPOLATION_HPP

/**
 * @file      intext.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      November, 2014
 *
 * @brief     Various functions to perform interpolation / extrapolation.
 *
 * @details   This file contains a number of functions to perform different
 *            types of interpolation and/or extrapolation. Some of the 
 *            functions use unit-offset arrays and NOT zero-offset as in C/C++.
 *            These are usually named as _somefunction_(...args...) while 
 *            their zero-offset wrappers are named somefunction(...args...). 
 *            Do not mix and match!
 *
 * @note      This library must be compiled under the C++11 standard.
 *
 * @todo      Need to test and provide a test_intext.cpp file, showing usage.
 *
 * @copyright Copyright © 2015 Dionysos Satellite Observatory, <br>
 *            National Technical University of Athens. <br>
 *            This work is free. You can redistribute it and/or modify it under
 *            the terms of the Do What The Fuck You Want To Public License,
 *            Version 2, as published by Sam Hocevar. See http://www.wtfpl.net/
 *            for more details.
 *
 * <b><center><hr>
 * National Technical University of Athens <br>
 *      Dionysos Satellite Observatory     <br>
 *        Higher Geodesy Laboratory        <br>
 *      http://dionysos.survey.ntua.gr
 * <hr></center></b>
 *
 */

#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace nmath {

/*! allocate a vector with subscript range v[nl..nh] */
template<class T>
inline T* _nr_vector_ (const long& nl,const long& nh)
{
  try {
    T* v = new T [nh-nl];
    return v-nl;
  } catch (std::bad_alloc&) {
    return nullptr;
  }
}

/*! free a nr_vector (allocated with nr_vector) */
template<class T>
inline void _free_nr_vector_ (T *v,const long& nl,const long& nh)
{
  if (v+nl) delete [] (v+nl);
}

/**
 * @brief Polynomial interpolation using Neville's algorithm (unit offset).
 */
int _polyneville_ (const double* xa,const double* ya,const int& n,
  const double& x,double& y,double& dy);

/**
 * @brief   Polynomial interpolation using Neville's algorithm (zero offset).
 * @details This is a wrapper function for _polyneville_ using zero-offset
 *          input arrays.
 * @see _polyneville_
 */
inline int polyneville (const double* xa,const double* ya,const int& n,
  const double& x,double& y,double& dy)
{
  return _polyneville_ (xa-1,ya-1,n,x,y,dy);
}

/**
 * @brief Rational Function interpolation (unit offset).
 */
int _rationalinterp_ (const double* xa,const double* ya,const int& n,
  const double& x,double& y,double& dy);

/**
 * @brief   Rational Function interpolation (zero offset).
 * @details This is a wrapper function for _rationalinterp_ using zero-offset
 *          input arrays.
 * @see _rationalinterp_
 */
inline int rationalinterp (const double* xa,const double* ya,const int& n,
  const double& x,double& y,double& dy)
{
  return _rationalinterp_ (xa-1,ya-1,n,x,y,dy);
}

/**
 * @brief Cubic Spline Interpolation (unit offset).
 */
int _csplineint_ (const double* xa,const double* ya,const double* y2a,const int& n,
  const double& x,double& y);

/**
 * @brief Cubic Spline Interpolation (zero offset).
 * @details This is a wrapper function for _csplineint_ using zero-offset
 *          input arrays.
 * @see _csplineint_ , preparecspline
 * @warning the input array y2a should have been allocated and created via the
 *          preparecspline function.
 */
inline int csplineint (const double* xa,const double* ya,const double* y2a,const int& n,
  const double& x,double& y)
{
  return _csplineint_ (xa-1,ya-1,y2a,n,x,y);
}

/**
 * @brief Prepare an array of second derivatives for cubic spline interpolation (unit offset).
 */
int _preparecspline_ (const double* x,const double* y,const int& n,double* y2,
  double yp1=1e30,double ypn=1e30);

/**
 * @brief Prepare an array of second derivatives for cubic spline interpolation (zero offset).
 * @details This is a wrapper function for _preparecspline_ using zero-offset
 *          input arrays.
 * @see csplineint , _preparecspline_
 */
inline int preparecspline (const double* x,const double* y,const int& n,double* y2,
  double yp1=1e30,double ypn=1e30)
{
  return _preparecspline_ (x-1,y-1,n,y2,yp1,ypn);
}

}; /* end namespace */

#endif
