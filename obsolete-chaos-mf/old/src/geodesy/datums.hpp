#ifndef _DATUMS_HPP
#define _DATUMS_HPP

/**
 * @file      datums.hpp
 * @version   1.0-1b
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 * @date      October, 2014
 *
 * @brief     A class to represent an ellipsoid.
 *
 * @details   This file contains a clas, to represent an ellipsoid.
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

#include <string>

namespace geo {

/**
 * @brief an ellipsoid class.
 *
 * @details class to define and describe an ellipsoid. Every ellipsoid is
 *          defined by two parameters, namely semi-major axis and flattening.
 *
 */
class Ellipsoid 
{
  public:
    /*! constructor; default ellipsoid is GRS80 */
    Ellipsoid();
    /*! constructor from ellispoid name */
    Ellipsoid(const char*);
    /*! a user-defined ellipsoid */
    Ellipsoid(double& a,double& f);

    /*! get flatening */
    inline double flattening() const {return f;}
    /*! get semi-major axis */
    inline double semimajor() const {return a;}
    /*! get inverse flatening */
    inline double inv_flattening() const {return 1.0e00/f;}
    /*! get semi-minor axis */
    inline double semiminor() const {return a-a*f;}
    /*! get eccentricity squared */
    inline double eccentricity2() const {return 2.0*f-f*f;}

  private:
    double a;         /**< semi-major axis */
    double f;         /**< flattening */
    std::string name; /**< name of the ellipsoid */
};

}; /* end of geo namespace */

#endif
