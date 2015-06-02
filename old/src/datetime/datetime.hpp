#ifndef _DATETIME_HPP
#define _DATETIME_HPP

/**
 * @file      datetime.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      October, 2014
 *
 * @brief     Functions for manipulating datetime.
 *
 * @details   This file contains a variety of functions to manipulate date and/or
 *            time in various formats and systems. Most of the functions are
 *            based on the corresponding SOFA \cite sofa routines. For the making
 *            of this library, SOFA release 2013-12-02 (C-version) was used. You
 *            can check the latest SOFA release at http://www.iausofa.org/current.html
 *
 * @note      Original SOFA \cite sofa source code written in ANSI C. For the 
 *            most part, functions in this file are translated to C++.
 *            The functions \c cal2jd and \c dtf2d have been modified so as to
 *            handle Modified Julian Dates.
 *
 * @pre       Note that the file gencon.hpp must be included. This file contains
 *            a list of needed variables (e.g. PI).
 *  
 * @attention Most of the routines are closely related to the SOFA \cite sofa
 *            package. Whenever a new SOFA release is issued, some of these 
 *            routines may have to be updated.
 *
 * @todo      
 *  -# optimize code for reading / writting dates as YMDHMS
 *     (e.g. d2dtf)
 *  -# optimize code for transforming from and for seconds to improve 
 *     accuracy (e.g. d2tf)
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
#include "gencon.hpp"

namespace datetime {

/**
 * @brief Truncate to nearest whole number towards zero.
 */
inline int dint(const double& d) {return ((d)<0.0?ceil(d):floor(d));}

/**
 * @brief Gregorian Calendar to Modified Julian Date.
 */
int cal2jd(const int& iy,const int& im,const int& id,
          double& djm0,double& djm);

/**
 * @brief Gregorian Calendar to Julian Date.
 */
int cal2jd_(const int& iy,const int& im,const int& id,
          double& djm0,double& djm);

/**
 * @brief Format for output a 2-part Julian Date.
 */
int d2dtf(const char *scale,const int& ndp,const double& d1,const double& d2,
          int& iy,int& im,int& id,int ihmsf[4]);

/**
 * @brief Decompose days to hours, minutes, seconds, fraction.
 */
void d2tf(const int& ndp,const double& days, 
      char* sign,int ihmsf[4]);

/**
 * @brief For a given UTC date, calculate delta(AT) = TAI-UTC.
 */
int dat(const int& iy,const int& im,const int& id,const double& fd,
      double& deltat);

/**
 * @brief Encode date and time fields into 2-part Modified Julian Date.
 */
int dtf2d(const char *scale,const int& iy,const int& im,const int& id,
  const int& ihr,const int& imn,const double& sec,
  double& d1,double& d2);

/**
 * @brief Encode date and time fields into 2-part Julian Date.
 */
int dtf2d_(const char *scale,const int& iy,const int& im,const int& id,
  const int& ihr,const int& imn,const double& sec,
  double& d1,double& d2);

/**
 * @brief Julian Date to Gregorian year, month, day, and fraction of a day.
 */
int jd2cal(const double& dj1,const double& dj2,
  int& iy, int& im,int& id,double& fd);

/**
 * @brief Julian Date to Gregorian Calendar, expressed in a form convenient for formatting messages.
 */
int jdcalf(const int& ndp,const double& dj1,const double& dj2,
  int iymdf[4]);

/**
 * @brief Convert hours, minutes, seconds to days.
 */
int tf2d(char s,const int& ihour,const int& imin,const double& sec,
  double& days);

}; // end namespace datetime

#endif
