#ifndef _GNSSOBSERVABLES_HPP
#define _GNSSOBSERVABLES_HPP

/**
 * @file      gstructs.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      October, 2014
 *
 * @brief     Classes and functions for manipulating gnss observation types.
 *
 * @details   This file contains a list of classes, enums and functions for
 *            manipulating gnss observation types. It is written to assist the 
 *            processing and analysis of Global Navigation Satellite Systems data.
 *            According to @cite rnx302 , every observable is uniquely defined 
 *            by a 3 parameters, i.e. observation type (t), band (n) and 
 *            attribute (a). Note that there are two more pseudo-observables,
 *            namely Ionosphere delay and Channel numbers which are also accounted
 *            for.
 *
 * @pre       Needs the library datetime::date and library varutils.
 *
 * @todo      Provide example usage code.
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

namespace ngpt {

/**
 * @details This class is used to represent any raw GNSS observable (that can be
 *          found in a RINEX file). It follows thr RINEX v3.xx convention, that
 *          every observable is uniquely defined by 3 parameters (type, band and attribute)
 *          accompanied by the satellite system.
 *          This class is simple and can only hold raw (i.e. RINEX) observables.
 *          To represent linear combinations use the class ngpt::observable.
 *
 */
class raw_obs {

  public:

    /*! Default constructor; all initialized to ::Unknown if not provided */
    raw_obs (ngpt::SatelliteSystem s=ngpt::SatelliteSystem::Unknown,
             ngpt::ObservationType t=ngpt::ObservationType::Unknown,
             ngpt::CarrierBand     n=ngpt::CarrierBand::Unknown,
             ngpt::TrackingCode    a=ngpt::TrackingCode::Unknown)
      :ms(s),mt(t),mn(n),ma(a){};

    inline ngpt::SatelliteSystem& satellitesystem () {return ms;}
    inline ngpt::SatelliteSystem  satellitesystem () const {return ms;}
    inline ngpt::ObservationType& type            () {return mt;}
    inline ngpt::ObservationType  type            () const {return mt;}
    inline ngpt::CarrierBand&     band            () {return mn;}
    inline ngpt::CarrierBand      band            () const {return mn;}
    inline ngpt::TrackingCode&    attribute       () {return ma;}
    inline ngpt::TrackingCode     attribute       () const {return ma;}

    /*! Set all members of instance to unknown */
    inline void setunknown () {
      ms = ngpt::SatelliteSystem::Unknown;
      mt = ngpt::ObservationType::Unknown;
      mn = ngpt::CarrierBand::Unknown;
      ma = ngpt::TrackingCode::Unknown;
    }

    /*! Resolve a raw_observable from a RINEX V2 string (e.g. 'L1') */
    static std::vector<raw_obs> resolve_v2x (const std::string&,std::vector<SatelliteSystem>* ss=nullptr);

    /*! Resolve a raw_observable from a RINEX V3 string (e.g 'C2L') */
    static raw_obs resolve_v3x (const std::string&,const ngpt::SatelliteSystem&,int&);

  private:

    ngpt::SatelliteSystem ms; ///< satellite system
    ngpt::ObservationType mt; ///< observation type
    ngpt::CarrierBand     mn; ///< carrier band
    ngpt::TrackingCode    ma; ///< tracking code / attribute
};

/**
 * @details This class is used to represent any GNSS observable, including
 *          any linear combination of the observables themselves. This class 
 *          should be used whenever we want to manipulate observables. Every instant
 *          of this class, is represented by a collection of raw observables (raw_obs)
 *          and their respective coefficients. There is no limit in the combinations
 *          this class can represent (multi-frequency, multi-gnss). This class is a
 *          generalization of the class ngpt::raw_obs.
 *
 * @see raw_obs
 */
class Observable {

  typedef std::pair<raw_obs,double> oc_pair; ///< Pair of type : <raw_obs, coefficient>
  typedef std::vector< oc_pair >    lcvec;   ///< A vector of pairs <raw_obs, coefficient>

  public:

    /*! Default constructor; all initialized to ::Unknown if not provided */
    Observable(ngpt::SatelliteSystem s=ngpt::SatelliteSystem::Unknown,
             ngpt::ObservationType t=ngpt::ObservationType::Unknown,
             ngpt::CarrierBand     n=ngpt::CarrierBand::Unknown,
             ngpt::TrackingCode    a=ngpt::TrackingCode::Unknown)
    :mvec ( 1,oc_pair (raw_obs(s,t,n,a),1.0e0) ) {}

    /*! Constructor from raw_obs */
    Observable (const raw_obs& o) : mvec ( 1,oc_pair (o,1.0e0) ) {}

    /*! (deep) Copy constructor */
    Observable (const Observable& o) : mvec ( o.mvec ) {}

    /*! Assignment operator */
    Observable& operator=(const Observable& o) {
      if (this!=&o) mvec = o.mvec;
      return *this;
    }

    /*! Move constructor */
    Observable (Observable&& o) : mvec (std::move(o.mvec)) {}

    /*! Move assignment operator */
    Observable& operator=(const Observable&& o) {
      mvec = std::move (o.mvec);
      return *this;
    }

    /**
     * @brief Resolve an observable from a RINEX V2 string
     * @note This function is a wrapper for raw_obs::resolve_v2x to
     *       handle observable type.
     * @see  raw_obs::resolve_v2x
     */
    inline static std::vector<Observable> resolve_v2x (const std::string& str,std::vector<SatelliteSystem>* ss=nullptr)
    {
      std::vector<raw_obs> ro = raw_obs::resolve_v2x (str,ss);
      std::vector<Observable> rtn;
      for (auto& i : ro) rtn.push_back (i);
      return rtn;
    }

    /**
     * @brief Resolve an observable from a RINEX V3 string 
     * @note This function is a wrapper for raw_obs::resolve_v3x
     *       handle observable type.
     * @see  raw_obs::resolve_v3x
     */
    inline static Observable resolve_v3x (const std::string& str,const ngpt::SatelliteSystem& s,
                                          int& status)
    {
      return Observable (raw_obs::resolve_v3x (str,s,status));
    }

  private:

    /**
     * @details This vector holds pairs of raw observable types and their
     *          respective coefficients. To represent any combination of
     *          the form e.g. LC = a * L1 + b * L2 + c * P1, the mvec will
     *          hold the pairs: <L1,a>, <L2,b>, <P1,c>.
     */
    lcvec mvec;

};

}; /* end of namespace */

#endif
