#ifndef _RINEX_HPP
#define _RINEX_HPP

/**
 * @file      rinex.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      November, 2014
 *
 * @brief     Classes and functions for manipulating input rinex files.
 *
 * @details   This file contains a list of classes, enums and functions for
 *            manipulating gnss rinex files. It is written to assist the 
 *            processing and analysis of Global Navigation Satellite Systems data.
 *
 * @todo 
 *  -# std::move (std::ifstream) does not work (it fails at compilation)! This
 *     fact disables move semantics, i.e. move constructor and move assignement
 *     Rinex (Rinex&& r) and Rinex& operator=(Rinex&& r).
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

#include "dates.hpp"
#include "varutils.hpp"

namespace ngpt {

/**
 * @details A rinex class used to represent a rinex file stream; it can be any of
 *          Observation Rinex (ngpt::ObsRinex),<br>
 *          Navigation Rinex (ngpt::NavRinex),<br>
 *          Meteorological Rinex (ngpt::MetRinex),<br>
 *          Clock Rinex (ngpt::ClkRinex)<br>
 *          A Rinex instance can only be created by an inherited class.
 */
class Rinex {

  public:

    /*! Move constructor */
    Rinex (Rinex&& r) = delete;
    /*Rinex (Rinex&& r):mfilename(std::move (r.mfilename)),mstream(std::move (r.mstream)),
      mtimesystem(r.mtimesystem),mfirstdate(r.mfirstdate),mlastdate(r.mlastdate){}*/

    /*! Move assignment operator */
    Rinex& operator=(Rinex&& r) = delete;
    /*Rinex& operator=(Rinex&& r) {
      mfilename   = std::move (r.mfilename);
      mstream     = std::move (r.mstream);
      mtimesystem = std::move (r.mtimesystem);
      mfirstdate  = std::move (r.mfirstdate);
      mlastdate   = std::move (r.mlastdate);
      return *this;
    }*/

    /*! Access the time system */
    datetime::TIME_SYSTEM timesystem() const {return mtimesystem;}

    /*! Access the time of first data record */
    datetime::Date firstdate() const {return mfirstdate;}

    /*! Access the time of last data record */
    datetime::Date lastdate() const {return mlastdate;}

    /*! Return the filename */
    std::string filename () const {return mfilename;}

    /*! Check if line is comment (using the columns 60-80) */
    static bool is_rinex_commentline (const std::string&);

  protected:


    /*! Default constructor given the input RINEX filename */
    Rinex(const std::string& f):mfilename(f),mstream(f,std::ifstream::in),
        mtimesystem(datetime::TIME_SYSTEM::Unknown){}

    /*! Rinex(es) are non construction-copyable */
    Rinex (const Rinex& r) = delete;

    /*! Rinex(es) are non copyable */
    Rinex& operator=(const Rinex& r) = delete;

    /*! Destructor; responsible for closing the stream */
    virtual ~Rinex() {close_stream();}

    /* Return input stream's state */
    inline std::ios_base::iostate bufstate() const {return mstream.rdstate();}

    /*! Return the input buffer */
    inline std::ifstream& stream () {return mstream;}

    /*! Access the time system */
    datetime::TIME_SYSTEM& timesystem() {return mtimesystem;}
    /*! Access the time of first data record */
    datetime::Date& firstdate() {return mfirstdate;}
    /*! Access the time of last data record */
    datetime::Date& lastdate() {return mlastdate;}

  private:
    
    /*! Close the input stream */
    inline void close_stream() {if (mstream.is_open()) mstream.close();}

    std::string           mfilename;   ///< The RINEX filename
    std::ifstream         mstream;     ///< The input (file) stream
    datetime::TIME_SYSTEM mtimesystem; ///< The time system
    datetime::Date        mfirstdate;  ///< Epoch of first data record
    datetime::Date        mlastdate;   ///< Epoch of last data record

}; /* end Rinex class */

}; /* end of namespace */

#endif
