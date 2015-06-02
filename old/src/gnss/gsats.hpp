#ifndef _GNSSSATELLITES_HPP
#define _GNSSSATELLITES_HPP

/**
 * @file      gsats.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      November, 2014
 *
 * @brief     Classes and functions for manipulating gnss satellites.
 *
 * @details   This file contains a list of classes, enums and functions for
 *            manipulating gnss observation types. It is written to assist the 
 *            processing and analysis of Global Navigation Satellite Systems data.
 *            In this file, a satellite class is defined, to accomodate the manipulation
 *            of all GNSS satellites (i.e. gps,glonass,galileo,qzss,bds and sbas).
 *
 * @pre       Needs the library datetime::date and library varutils.
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

#include "gstructs.hpp"
#include "varutils.hpp"

#define _SAT_UNDF_ -999

namespace ngpt {

/**
 * @details A satellite class; to define a satellite we need the satellite system
 *          and its identity as described in Section 3.5 of @cite rnx302
 */
class Satellite {

  public:

    /*! Default constructor; all members set to zero and Unknown */
    Satellite():msatsys(SatelliteSystem::Unknown),mprn(_SAT_UNDF_),msvn(_SAT_UNDF_)
               ,mmodel(SatelliteBlockModel::Unknown){};
    
    /*! Resolve from RINEX string */
    Satellite (const std::string&);

    SatelliteSystem& satsys() {return msatsys;}
    SatelliteSystem satsys() const {return msatsys;}
    int& prn() {return mprn;}
    int prn() const {return mprn;}
    int& svn() {return msvn;}
    int svn() const {return msvn;}
    SatelliteBlockModel& blockmodel() {return mmodel;}
    SatelliteBlockModel blockmodel() const {return mmodel;}

    /*! Resolve a satellite from a RINEX satellite string */
    int setfromstr (const std::string&);

    /*! Set all members to _SAT_UNDF_ / Unknown */
    inline void initialize () {
      msatsys = SatelliteSystem::Unknown;
      mprn = _SAT_UNDF_;
      msvn = _SAT_UNDF_;
      mmodel = SatelliteBlockModel::Unknown;
    }

  private:

    SatelliteSystem     msatsys; ///< The satellite's system

    /**
     * @details This number represents:<br>
     *          the PRN for gps, galileo, bds<br>
     *          the slot number for glonass<br>
     *          prn-100 for sbas,qzss(L1-SAIF)<br>
     *          prn-192 for qzss<br>
     * @cite    rnx302 , Sec. 3.5
     */
    int                 mprn;

    int                 msvn;    ///< Space Vehicle Number
    SatelliteBlockModel mmodel;  ///< Satellite Block Model

}; /* end class Satellite */

}; /* end of namespace */

#endif
