#ifndef _CODEAIUB_HPP
#define _CODEAIUB_HPP

/**
 * @file      codeaiub.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      October, 2014
 *
 * @brief     Classes and functions for manipulating files and data from CODE AC.
 *
 * @details   This file contains a list of classes, enums and functions for
 *            manipulating and processing file, formats and data from Center for
 *            Orbit Determination in Europe (CODE) Analysis Center and Bernese
 *            GNSS Software input/output files.
 *
 * @pre       Needs the libraries datetime and vutils.
 *  
 *
 * @todo      
 *  -# Provide test cases, source examples; 
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

#include <vector>
#include <string>
#include "dates.hpp"
#include "varutils.hpp"

/**
 * @brief Class to hold a Bernese v5.2 .sta file.
 *
 * @details This class is used to represent and processes a station information
 *          file, as used in Bernese v5.2 GNSS Software. An example of such a
 *          file can be found here: ftp://ftp.unibe.ch/aiub/BSWUSER52/STA/CODE.STA
 *          The begining of the TYPES sections (1 through 5) are marked when initializing
 *          an instance, for further ease of use. The member variables mtype1
 *          through mtype5 correspong to the positions in the file when these
 *          sections start. E.g. mtype1 marks <b>the end of the line</b>:
 *          TYPE 001: RENAMING OF STATIONS
 *
 * @attention This class and its members depend on the Bernese station information
 *            file format (.STA files).
 *
 * @todo
 *  -# TYPE 001 (renaming) is not handled
 *  -# getstainf handle igs names with codes (in addition to 4digit names)
 *
 * @bug
 *  -# Clarify what flags mean
 *  -# Renaming of stations not handled! e.g.:
 * @verbatim
TYPE 001: RENAMING OF STATIONS
------------------------------

STATION NAME          FLG          FROM                   TO         OLD STATION NAME      REMARK
****************      ***  YYYY MM DD HH MM SS  YYYY MM DD HH MM SS  ********************  ************************
REYK 10202M001        001                                            REYK*                 STN
.
.
.
TYPE 003: HANDLING OF STATION PROBLEMS
--------------------------------------

STATION NAME          FLG          FROM                   TO         REMARK
****************      ***  YYYY MM DD HH MM SS  YYYY MM DD HH MM SS  ************************************************************
REYK_10202M001        003  1998 11 07 00 00 00  1998 11 09 23 59 59  %IGS_CRX:
@endverbatim
 * the function gettype3flag will not recognize that REYK_10202M001 and REYK are
 * the same station. Maybe use regular expressions ??
 *
 */
class Bernsta {

  public:
      /*! @brief Default constructor; does nothing. */
      Bernsta ();

      /*! @brief Constructor using the name of the file. */
      Bernsta (const std::string&);

      /*! @brief Destructor; close the stream. */
      ~Bernsta ();

      /*! @brief Set from file name */
      bool setfrom (const std::string&);

      /*! @brief Collect information for a certain date & station */
      int getstainf (const std::string&,const datetime::Date&,
        std::string&,std::string&,double&,double&,double&,int&,std::string&);

      /*! @brief Collect information flag from section TYPE 003 */
      int gettype3flag (const std::string&,const datetime::Date&,int&,std::string&);

  private:
    std::ifstream mbuf;      /**< input file stream */
    std::string   mfilename; /**< input file name */
    bool          streamopen;/**< flag to mark if the file is open */
    size_t        mtype1;    /**< position (in file) of eol: TYPE 001: RENAMING OF STATIONS */
    size_t        mtype2;    /**< position (in file) of eol: TYPE 002: STATION INFORMATION */
    size_t        mtype3;    /**< position (in file) of eol: TYPE 003: HANDLING OF STATION PROBLEMS */
    size_t        mtype4;    /**< position (in file) of eol: TYPE 004: STATION COORDINATES AND VELOCITIES (ADDNEQ) */
    size_t        mtype5;    /**< position (in file) of eol: TYPE 005: HANDLING STATION TYPES */
};

#endif
