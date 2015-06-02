#ifndef _GSITE_HPP
#define _GSITE_HPP

/**
 * @file      gsite.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      November, 2014
 *
 * @brief     Classes and functions for manipulating gnss sites &
 *            instrumentation/equipment.
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

#include <cstring>
#include "varutils.hpp"

/**
 * @details This definition is used to match the number of element to seperate the
 *          antenna type from the radome, i.e starting char of radome.
 *
 * @see     Antenna
 *
 * Reference http://igscb.jpl.nasa.gov/igscb/station/general/rcvr_ant.tab
 *
 */
#define _RADOME_START_AT_ 16

namespace ngpt {

/**
 * @details A receiver class. Receivers: 20 columns maximum
 *          One space between manufacturer name and model name
 *          Allowed in model name: A-Z and 0-9 and space and - +
 *          The receiver type of any instance is represented by a
 *          string of exactly 20 chars.
 *
 * Reference http://igscb.jpl.nasa.gov/igscb/station/general/rcvr_ant.tab
 *
 */
class Receiver {

  public:

    /*! Default constructor */
    Receiver(){};

    /*! Constructor from receiver type */
    explicit Receiver (const char*);

    /*! Constructor from receiver type */
    explicit Receiver (const std::string&);

    /*! Destructor */
    ~Receiver(){}

    /*! Copy constructor */
    Receiver (const Receiver& r) {
      std::memcpy ( mtype,r.mtype,21*sizeof(char) );
    }

    /*! Assignment operator */
    Receiver& operator=(const Receiver& r) {
      if (this!=&r) std::memcpy ( mtype,r.mtype,21*sizeof(char) );
      return *this;
    }
    /*! Assign from a string */
    Receiver& operator=(const char* c) {
      Receiver rtn (c);
      *this = rtn;
      return *this;
    }
    /*! Assign from a string */
    Receiver& operator=(const std::string& s) {
      Receiver rtn (s);
      *this = rtn;
      return *this;
    }

    /*! Return the receiver type ptr */
    inline char* type () {return mtype;}

    /*! Return the receiver type as string */
    inline std::string type_s () {return std::string (mtype);}

    /*! Check equality */
    inline bool operator==(const Receiver& r) {
      return ( ! strcmp (mtype,r.mtype) );
    }

  private:

    char mtype[20]; ///< The type of the receiver
};

/**
 * @details A class to holf GNSS antennas. Antennas:  15 columns maximum
 *          First three characters are manufacturer code (except satellites)
 *          Allowed in manufacturer code: - and A-Z and 0-9
 *          Allowed in model name: /-_.+ and A-Z and 0-9
 *          Model name must start with A-Z or 0-9
 *          Radomes:   4 columns; A-Z and 0-9 allowed
 *          Antenna+Radome: Combine them with the radome code in columns 17-20.  Fill
 *          with spaces between the end of the antenna and column 17.
 *          Example: AOAD/M_T        SCIT
 *
 * @note    If the given string describing the antenna has no radome type, then
 *          the constructor will replace whitespaces with radome type 'NONE'.
 *          All antennas should have a radome type, even if it is 'NONE'
 *
 * Reference http://igscb.jpl.nasa.gov/igscb/station/general/rcvr_ant.tab
 *
 * @todo Acoording to @cite rnx302 , an antenna as described in a RINEX file can 
 *       hold a lot more information (especially for moving markers).
 *       Some may need to be added.
 */
class Antenna {

  public:

    /*! Default constructor */
    Antenna(){};

    /*! Constructor from antenna type plus radome */
    explicit Antenna (const char*);

    /*! Constructor from antenna type plus radome */
    explicit Antenna (const std::string&);

    /*! Destructor */
    ~Antenna(){}

    /*! Copy constructor */
    Antenna (const Antenna& a) {
      std::memcpy (mtype,a.mtype,21*sizeof(char));
    }

    /*! Assignment operator */
    Antenna& operator=(const Antenna& a) {
      if (this!=&a) std::memcpy (mtype,a.mtype,21*sizeof(char));
      return *this;
    }
    /*! Assign from a string */
    Antenna& operator=(const char* c) {
      Antenna rtn (c);
      *this = rtn;
      return *this;
    }
    /*! Assign from a string */
    Antenna& operator=(const std::string& s) {
      Antenna rtn (s);
      *this = rtn;
      return *this;
    }

    /**
     * @brief   Return the antenna type ptr 
     * @warning The antenna type spans [0-_RADOME_START_AT_); after that comes the radome
     */
    inline char* type () {return mtype;}

    /*! Return the radome */
    inline char* radome () {return mtype+_RADOME_START_AT_;}

    /*! Return the antenna type as string */
    inline std::string type_s () {return std::string (mtype,_RADOME_START_AT_);}

    /*! Return the radome type as string */
    inline std::string radome_s () {return std::string (mtype+_RADOME_START_AT_,4);}

    /*! Check equality */
    inline bool operator==(const Antenna& a) {
      return ( ! strcmp (mtype,a.mtype) );
    }

  private:

    /*! Fix antenna's radome, from whitespace to 'NONE' */
    char* fix_radome ();

    /** 
     * The type of the antenna plus radome. The elements of the array
     * from [0-16) describe the antenna type (no terminating char allowed),
     * while elements from [16-20) describe the radome. mtype[20] is the
     * null terminating character. i.e. mtype holds an array of type:
     * [XXXXXXXXXXXXXXX DOME\0]
     */
    char mtype[21];

};

/**
 * @details A Site class. An instance of this class should be able to represent
 *          any GNSS site, with information about its instrumentation and
 *          various other vital (meta)data.
 */
class Site {

  public:

    /**
     * @brief The marker type, see @cite rnx302 Sec. 5.3
     *
     * @details The record is required except for GEODETIC and NON_GEODETIC marker 
     *          types. Attributes other than GEODETIC and NON_GEODETIC will tell the user 
     *          program that the data were collected by a moving receiver. The inclusion of 
     *          a 'start moving antenna' record (event flag 2) into the data body of the RINEX 
     *          file is therefore not necessary. Event flags 2 and 3 are still necessary to 
     *          flag alternating kinematic and static phases of a receiver visiting multiple earth-
     *          fixed monuments, however.
     */
    enum class MARKER_TYPE : char {
      Geodetic        ,///< Earth-fixed high-precision monument
      Non_Geodetic    ,///< Earth-fixed low-precision monument
      Non_Physical    ,///< Generated from network processing
      Space_borne     ,///< Orbiting space vehicle
      Air_borne       ,///< Aircraft, balloon, etc.
      Water_Craft     ,///< Mobile water craft
      Ground_Craft    ,///< Mobile terrestrial vehicle
      Fixed_Buoy      ,///< 'Fixed' on water surface
      Floating_Buoy   ,///< Floating on water surface
      Floating_Ice    ,///< Floating ice sheet, etc
      Glacier         ,///< 'Fixed' on a glacier
      Ballistic       ,///< Rockets, shells, etc
      Animal          ,///< Animal carrying a receiver
      Human            ///< Human being
    };

    /*! Default constructor; all initialized to default */
    Site(){};

    /*! Return the receiver */
    ngpt::Receiver& receiver () {return mreceiver;}

    /*! Return the atenna */
    ngpt::Antenna& antenna () {return mantenna;}

    /*! Return crd's; x component (meters) */
    inline float& x () {return mcrd[0];}
    /*! Return crd's; y component (meters) */
    inline float& y () {return mcrd[1];}
    /*! Return crd's; z component (meters) */
    inline float& z () {return mcrd[2];}

    /*! Return height of ARP above marker (meters) */
    inline float& dh () {return mdhen[0];}
    /*! Return east horizontal eccentricity (meters) */
    inline float& de () {return mdhen[1];}
    /*! Return north horizontal eccentricity (meters) */
    inline float& dn () {return mdhen[2];}

    /*! Set the name (i.e. mname) from a string */
    void setname (const std::string&);

    /*! Set the number (i.e. mnumber) from a string */
    void setnumber (const std::string&);

    /*! Set the marker type (i.e. mtype) from a string */
    void settype (const std::string&);

    /*! Set the site's id; first 4 chars of mname */
    inline void setidfromname () {
      std::memcpy (mid,mname,4*sizeof(char));
      mid[4] = '\0';
    }

  private:

    ngpt::Antenna mantenna;  ///< The antenna
    ngpt::Receiver mreceiver;///< The receiver
    float mcrd[3];           ///< Geocentric, cartesian coordinates (meters)
    float mdhen[3];          ///< Height of ARP above marker; Horizontal eccentricity (east north) in meters
    char  mid[5];            ///< 4-char id name of Site
    char  mname[21];         ///< The name of the site
    char  mnumber[21];       ///< The marker number
    MARKER_TYPE mtype;       ///< The marker type
};

}; /* end of namespace */

#endif
