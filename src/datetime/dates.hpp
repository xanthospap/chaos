#ifndef _DATES_HPP
#define _DATES_HPP

/**
 * @file      dates.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      October, 2014
 *
 * @brief     Classes and functions for manipulating datetime.
 *
 * @details   This file contains a list of classes, enums and functions for
 *            manipulating datetime events. It is written to assist the processing
 *            and analysis of Global Navigation Satellite Systems data.
 *
 * @pre       Needs the library vutils for function date::fromymdstr.<br>
 *            The user must define the precision of the library at <b>compile time</b>, using
 *            the DT_PRC_* macro (e.g. g++ ... -DDT_PRC_NANO ...)
 *  
 * @attention When printing dates in YY-MM-DD:HH:MM:SEC format, do not use over 
 *            9 decimal digits.
 *
 * @todo 
 * -# Define (in some way) the accuracy of the library.
 * -# Document and check the datetime::date::rearange function.
 * -# Time systems, TIME_SYSTEM::GAL, TIME_SYSTEM::BDT and TIME_SYSTEM::QZS are
 *    are defined but their properties are unknow. Need to check which time system
 *    they follow (e.g. UTC, TT, ..), and if needed add code for manipulating them.
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

/** @example test_dates.cpp
 *  Example usage of datetime library.
 */

#include <cmath>
#include <iostream>
#include <fstream>
#include "datetime.hpp"
#include "gencon.hpp"

/**
 * @brief Define precision at compile time.
 * @def DT_PRC_MILLI 
 *      Define precision at the MilliSecond level (1e<sup>-03</sup>sec, or 1e<sup>-08</sup>MJD).
 * @def DT_PRC_MICRO 
 *      Define precision at the MicroSecond level (1e<sup>-06</sup>sec, or 1e<sup>-11</sup>MJD).
 * @def DT_PRC_NANO 
 *      Define precision at the NanoSecond level (1e<sup>-09</sup>sec, or 1e<sup>-14</sup>MJD).
 * @def DT_PRC_PICO 
 *      Define precision at the PicoSecond level (1e<sup>-12</sup>sec, or 1e<sup>-17</sup>MJD).
 * @def DT_PRC_FEMTO 
 *      Define precision at the FemtoSecond level (1e<sup>-15</sup>sec, or 1e<sup>-20</sup>MJD).
 * @def SEC_PRC
 *      The precision in seconds.
 * @def JD_PRC
 *      The precision in JD (or MJD).
 */
#ifdef DT_PRC_MILLI
  #define SEC_PRC 1e-03
  #define JD_PRC  1e-08
#elif DT_PRC_MICRO
  #define SEC_PRC 1e-06
  #define JD_PRC  1e-11
#elif DT_PRC_NANO
  #define SEC_PRC 1e-09
  #define JD_PRC  1e-14
#elif DT_PRC_PICO
  #define SEC_PRC 1e-12
  #define JD_PRC  1e-17
#elif DT_PRC_FEMTO
  #define SEC_PRC 1e-15
  #define JD_PRC  1e-20
#endif

namespace datetime {

/**
 * @brief Return the precision of the library in seconds
 */
inline double datetime_precision_secs () {return SEC_PRC;}

/**
 * @brief Return the precision of the library in days
 */
inline double datetime_precision_days () {return JD_PRC;}

/**
 * @brief Enumeration type to define a possible time system.
 */
enum class TIME_SYSTEM : unsigned char {
  Unknown,   ///< Unknown or Undefined system
  UTC,       ///< Coordinated Universal Time (UTC)
  UT1,       ///< Universal Time (UT1)
  TAI,       ///< International Atomic Time (TAI)
  TT,        ///< Terestrial Time (TT)
  GPS,       ///< GPS time system
  GLO,       ///< Glonass time system
  GAL,       ///< Galileo time system
  BDT,       ///< Beidou time system
  QZS,       ///< QZSS time system
};

/**
 * @brief Enumeration type to define a possible time format.
 */
enum class TIME_FORMAT : unsigned char {
  Unknown,              /**< Unknown or Undefined format */
  JULIAN_DATE,          /**< Julian Date */
  MODIFIED_JULIAN_DATE, /**< Modified Julian Date (note that MJD=JD-(2400000.5=DJM0)) */
  YDMHMS                /**< Calendar date format (YYYY-MM-DD:HH:MM:SS)*/
};

/*! string to time system */
TIME_SYSTEM string2timesystem (const std::string&, int&);

/*! time system to string */
std::string timesystem2string (const TIME_SYSTEM&);

/**
 * @brief Two-digit year to four digit
 */
inline int tdy2fdy (const int& y){return ((y<80)?(2000+y):(1900+y));}

/**
 * @brief Date class.
 *
 * @details A date (actually datetime) class. Internally all dates are stored
 * as Modified Julian Dates split into two parts: \p md1 the "big" integer part
 * and \p md2 the fractional part, i.e. \c md1 is the Modified Julian Day and
 * \c md2 is the fraction of day.
 */
class Date
{
  public:

    /*! Empty constructor; produces J2000.0 */
    Date();

    /*! Constructor from Julian Date or Modified Julian Date */
    Date(double d1,double d2=0.0e0,
      TIME_FORMAT tf=TIME_FORMAT::JULIAN_DATE,TIME_SYSTEM ts=TIME_SYSTEM::Unknown);

    /*! Constructor from  Calender format (i.e. YDMHMS) */
    Date(const int& iy,const int& im,const int& id,int ihr=0,int imn=0,double sec=.0,
      TIME_SYSTEM ts=TIME_SYSTEM::Unknown);

    /*! Destructor */
    ~Date(){};

    /**
     * Read in date from a string of type: YYYY MM DD HH MM SS.SSS
     *                                 or:   YY MM DD HH MM SS.SSS
     */
    int fromymdstr (const std::string&,TIME_SYSTEM ts=TIME_SYSTEM::Unknown);

    /*! Set date to now */
    int setnow ();

    /*! Transform a date to a string of type: YYYY/MM/DD HH:MM::SS.SSS */
    std::string toymdstr(int& status,int ndp=3) const;

    /**
     * Rearange the two values (\p md1,\p md2) so that the first
     * is an integer and the second is a the decimal part
     */
    void rearange();

    /*! Access \p md1 */
    inline double&      d1 () {return md1;}
    /*! Get \p md1 */
    inline double       d1 () const {return md1;}
    /*! Access \p md2 */
    inline double&      d2 () {return md2;}
    /*! Get \p md2 */
    inline double       d2 () const {return md2;}
    /*! Access \p mtsys */
    inline TIME_SYSTEM& timesystem() {return mtsys;}
    /*! Get \p mtsys */
    inline TIME_SYSTEM  timesystem() const {return mtsys;}

    /*! Return the Modified Julian Date */
    inline double mjd () const {return md1 + md2;}

    /*! Return the Julian Date */
    inline double jd () const {return (md1+DJM0) + md2;}

    /**
     * @brief    Overload operator ">".
     * @warning  No time system checks implemented.
     * @note     As all comparissons, depends on wanted precision.
     */
    inline bool operator> (const Date& d) const {
      return (md1-d.md1) + (md2-d.md2) > JD_PRC;
      /*&& m_system==d.m_system;*/
    }

    /**
     * @brief    Overload operator ">=".
     * @warning  No time system checks implemented.
     * @note     As all comparissons, depends on wanted precision.
     */
    inline bool operator>=(const Date& d) const {
      return (md1-d.md1) + (md2-d.md2) >= JD_PRC;
      /*&& m_system==d.m_system;*/
    }

    /**
     * @brief    Overload operator "<".
     * @warning  No time system checks implemented.
     * @note     As all comparissons, depends on wanted precision.
     */
    inline bool operator< (const Date& d) const {
      return (md1-d.md1) + (md2-d.md2) < JD_PRC;
      /*&& m_system==d.m_system;*/
    }

    /**
     * @brief    Overload operator "<=".
     * @warning  No time system checks implemented.
     * @note     As all comparissons, depends on wanted precision.
     */
    inline bool operator<=(const Date& d) const {
      return (md1-d.md1) + (md2-d.md2) <= JD_PRC;
      /*&& m_system==d.m_system;*/
    }

    /**
     * @brief    Overload operator "==".
     * @warning  No time system checks implemented.
     * @note     As all comparissons, depends on wanted precision.
     */
    inline bool operator==(const Date& d) const {
      return fabs ((md1-d.md1) + (md2-d.md2)) < JD_PRC;
      /*&& m_system==d.m_system;*/
    }

    /**
     * @brief    Overload operator "!=".
     * @warning  No time system checks implemented.
     * @note     As all comparissons, depends on wanted precision.
     */
    inline bool operator!=(const Date& d) const {
      return !(*this==d);
    }

    /**
     * @brief   Binary write to an std::fstream
     * @warning Stream is not checked! 
     */
    inline void write(std::fstream& s) {
      s.write((char*)&md1,sizeof(double));
      s.write((char*)&md2,sizeof(double));
    }

    /** 
     * @brief    Binary read from an std::fstream
     * @warning  Stream is not checked!
     */
    inline void read (std::fstream& s) {
      s.read((char*)&md1,sizeof(double));
      s.read((char*)&md2,sizeof(double));
    }

    /*! Add seconds to a date */
    void addsec(const double& sec);

    /** 
     * @brief   Difference of two dates in seconds.
     * @details This function returns the time interval between two dates in 
     *          seconds.
     * @param[in] d The minuend date in subtraction (\c datetime::date).
     * @return      The time interval from the subtraction: <tt>d - this</tt> 
     *              in seconds (\c double).
     */
    inline double deltasec(const Date& d) const {
      return fabs (md1-d.md1) <= JD_PRC ?
             (md2-d.md2) * DAYSEC :
            ((md2-d.md2) * DAYSEC) + (dint(md1-d.md1) * DAYSEC);
    }

    /**
     * @brief   Seconds from the beggining of day.
     * @details Return the seconds from the begining of the day.
     * @return  Seconds from the beggining of day (\c double).
     */
    inline double secofday() const {return md2 * DAYSEC;}

  private:

    /*! first of a 2-part Julian Date; this is the integer part */
    double      md1;
    /*! second of a 2-part Julian Date; this is the decimal part */
    double      md2; 
    /* the date's time system */
    TIME_SYSTEM mtsys;

}; /* end of date class */

}; /* namespace datetime */

#endif
