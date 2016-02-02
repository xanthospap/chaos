#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "codeaiub.hpp"

using std::string;

/**
 * @details This function will scan an std::ifstream (i.e. open input file) and mark
 *          the positions in the file where the following lines apear:
 *          line (exact match)                                    | position identifier
 *          ------------------------------------------------------|-------------
 *          TYPE 001: RENAMING OF STATIONS                        | t1
 *          TYPE 002: STATION INFORMATION                         | t2
 *          TYPE 003: HANDLING OF STATION PROBLEMS                | t3
 *          TYPE 004: STATION COORDINATES AND VELOCITIES (ADDNEQ) | t4
 *          TYPE 005: HANDLING STATION TYPES                      | t5
 *          This function is used to initialize a bernsta instance. The lines fit
 *          the format of a Bernese v5.2 station information file. No line is
 *          considered a match, except if it matches exactly the ones specified
 *          above (ignoring leading and trailing whitespaces).
 *
 * @param[in] fin            an input file stream (\c std::ifstream).
 * @param[in] t1,t2,t3,t4,t5 position in file of the wanted lines (\c size_t).
 * @return                   an integer denoting the status of the function (\c integer);
 *                           this can be:
 * Return Value | Status
 * -------------|------------------------
 * -1           | stream error
 *  0           | success; all positions marked
 *  1           | failed to match 1<sup>st</sup> position; all next positions ambiguous
 *  2           | failed to match 2<sup>st</sup> position; all next positions ambiguous
 *  3           | failed to match 3<sup>st</sup> position; all next positions ambiguous
 *  4           | failed to match 4<sup>st</sup> position; all next positions ambiguous
 *  5           | failed to match 5<sup>st</sup> position
 *
 * @attention The functions marks the position at the end of the matched line. Positioning
 *            the stream at the marked position, the first line to be read is the
 *            line following the matched one. For example, if (part) of the file
 *            is:
 * @verbatim
ZWEN_12330M001        003  2000 04 01 00 00 00  2001 11 17 23 59 59  %EPN_SNX
ZWEN_12330M001        003  2004 01 01 00 00 00  2004 06 05 23 59 59  %EPN_SNX


TYPE 004: STATION COORDINATES AND VELOCITIES (ADDNEQ)
-----------------------------------------------------
                                            RELATIVE CONSTR. POSITION     RELATIVE CONSTR. VELOCITY
STATION NAME 1        STATION NAME 2        NORTH     EAST      UP        NORTH     EAST      UP
****************      ****************      **.*****  **.*****  **.*****  **.*****  **.*****  **.*****
@endverbatim
 * then positioning the stream at t4 the first line to be read is
 * @verbatim
-----------------------------------------------------
@endverbatim
 *
 */
int sta52_initialize(std::ifstream& fin,size_t& t1,size_t& t2,size_t& t3,
    size_t& t4,size_t& t5)
{
  string line;
  std::vector<string> strvec;
  
  /* initialize all positions to 0 */
  t1 = t2 = t3 = t4 = t5 = 0;

  /* matched types */
  std::vector<bool> matched (5,false);

  /* make sure stream is open and in good state */
  if (!fin.is_open()) {
    //std::cerr<<"***station information file stream failed!";
    return -1;
  }

  /* go to start of file */
  fin.seekg (0,std::ios::beg);

  /* read first line */
  getline (fin,line);

  /* read in while not eof */
  while (!fin.eof()) {
    /* all string we are searching for have size > 5 */
    if (line.size()>5) {
      strvec = splitstr (line); // split the line into strings
      /* all string we are searching for have at least 2 words */
      if (strvec.size()>2) {
        /* all strings we are searching for begin with TYPE */
        if (strvec[0]=="TYPE") {
          /* the next word should be an integer in range [0-5] */
          try {
            int i = std::stoi (strvec[1]);
            switch (i) {
              case (1) :
                if (stripstr (line)=="TYPE 001: RENAMING OF STATIONS") {
                  t1 = fin.tellg();
                  matched[i-1] = true;
                  }
                break;
              case (2) :
                if (stripstr (line)=="TYPE 002: STATION INFORMATION") {
                  t2 = fin.tellg();
                  matched[i-1] = true;
                }
                break;
              case (3) :
                if (stripstr (line)=="TYPE 003: HANDLING OF STATION PROBLEMS") {
                  t3 = fin.tellg();
                  matched[i-1] = true;
                }
                break;
              case (4) :
                if (stripstr (line)=="TYPE 004: STATION COORDINATES AND VELOCITIES (ADDNEQ)"){
                  t4 = fin.tellg();
                  matched[i-1] = true;
                }
                break;
              case (5) :
                if (stripstr (line)=="TYPE 005: HANDLING STATION TYPES") {
                  t5 = fin.tellg();
                  matched[i-1] = true;
                }
                break;
              default:
                break;
            }
            if (i==5)
              break;
          } catch (...){} /*{
            i=-1;
          }*/
        }
      }
    }
    /* read next line */
    getline (fin,line);
  }

  /* return the status */
  auto it = std::find (matched.begin(),matched.end(),false);
  return (it == matched.end()) ? 0 : std::distance(matched.begin(),it) ;
}

/**
 * @details Default constructor; nothing is actually done, position flags are set
 *           to 0 and the stream is marked as closed (i.e. <tt>streamopen=false</tt>).
 *
 */
Bernsta::Bernsta ()
  : mfilename(""),
  streamopen(false),
  mtype1(0),mtype2(0),
  mtype3(0),mtype4(0),mtype5(0)
{
}

/**
 * @details Constructor using the name of the station information file. If the
 *          file fails to open, then the the stream is marked as closed (i.e. 
 *          <tt>streamopen=false</tt>). If the file is valid (i.e. can be opened),
 *          then the file is read and the coresponding positions of interest are
 *          marked for further use. If a position is not found, then its value is
 *          set to zero.
 *
 * @param[in] str The name of the input .STA file (\c std::string).
 *
 */
Bernsta::Bernsta (const std::string& str)
  : mfilename(str),
  streamopen(true),
  mtype1(0),mtype2(0),
  mtype3(0),mtype4(0),mtype5(0)
{
  /* open the file */
  mbuf.open (mfilename,std::ifstream::in);

  /* assign positions */
  int status = sta52_initialize (mbuf,mtype1,mtype2,mtype3,mtype4,mtype5);

  /* check the status */
  if (status==-1) streamopen = false;
}

Bernsta::~Bernsta()
{
  /* if the stream is open, close it */
  if (mbuf.is_open()) mbuf.close ();

  streamopen = false;
}

/**
 * @details Set an instance using the name of the station information file. If the
 *          file fails to open, then the the stream is marked as closed (i.e. 
 *          <tt>streamopen=false</tt>). If the file is valid (i.e. can be opened),
 *          then the file is read and the coresponding positions of interest are
 *          marked for further use. If a position is not found, then its value is
 *          set to zero. If the instance holds another .sta file, then all information
 *          is lost and the instance is re-initilized.
 *
 * @param[in] str The name of the input .STA file (\c std::string).
 * @return        True if file opened succesesfuly, false otherwise (\c bool).
 *
 * @note The returned status only informs for the state of the file (i.e. whether
 *       it is open or not). It has nothing to do with the search and set of the
 *       position flags (<tt>mtype1 through mtype5</tt>).
 *
 */
bool Bernsta::setfrom (const std::string& str)
{
  /* set all positions to zero */
  mtype1 = mtype2 = mtype3 = mtype4 = mtype5 = 0;

  /* if for some reason the stream is open, close it */
  if (mbuf.is_open()) mbuf.close ();
  streamopen = false;

  /* set the filename */
  mfilename = str;

  /* open the stream */
  mbuf.open (mfilename,std::ifstream::in);

  /* assign positions */
  int status = sta52_initialize (mbuf,mtype1,mtype2,mtype3,mtype4,mtype5);

  /* check the status */
  if (status==-1) streamopen = false;

  /* return */
  return streamopen;
}

/**
 * @details Gather information for a specific station for a specific epoch from 
 *          TYPE 002 section of the station information file. The 
 *          antenna displacement vector [dn,de,du] is returned in meters 
 *          (as in the input file). The information to collect are:
 *          Variable | Explanation                           | Comments
 *          ---------|---------------------------------------|------------------
 *          receiver | RECEIVER TYPE                         | -
 *          antenna  | ANTENNA TYPE (+ RADOME)               | antenna type plus radome (if any)
 *          n        | ARP north eccentricity                | meters
 *          e        | ARP east eccentricity                 | meters
 *          u        | ARP up eccentricity                   | meters
 *          flag     | the flag of the station               | -
 *          remark   | the remark/comment at the end of line | may be empty
 *
 * @param[in]  str      The name of the station (only name - code is dergarded) 
 *                      (\c std::string).
 * @param[in]  d        The epoch for which the information is requested 
 *                      (\c datetime::Date).
 * @param[out] receiver The receiver name as in the ,sta file (\c std::string).
 * @param[out] antenna  The name name (+ radome) as in the ,sta file (\c std::string).
 * @param[out] n,e,u    ARP eccentricity in north, east and up components in 
 *                      meters (\c double).
 * @param[out] flag     The flag used to mark the station for this epoch (\c int).
 * @param[out] remark   The comment (if any) ath the end of the line (\c std::string).
 * @return              An integer deonoting the exit status, which can be:
 *                      Retuned Integer | Status
 *                      ----------------|---------------------
 *                                   -3 | TYPE 002: STATION INFORMATION not set (i.e. mtype2=0)
 *                                   -2 | invalid file format (unresolved date string)
 *                                   -1 | steam failure
 *                                    0 | all ok
 *                                    1 | station not found
 *                                    2 | station not found for this interval
 *                                    3 | -
 *                                    4 | could not collect info (i.e. invalid info line)
 *
 * @note A remark/comment at the end of a TYPE 002 line is optional; hence, if
 *       no remark is found, no error flag is set and the line is considered correct
 *       (i.e. the return code is independent from finding or not of the remark).
 *
 */
int Bernsta::getstainf (const std::string& str,const datetime::Date& d,
  string& receiver, string& antenna,
  double& n, double& e, double& u,int& flag, std::string& remark)
{
  /* quick return if stream is invalid */
  if ((!streamopen) || (!mbuf.is_open ()))
    return -1;

  /* check that we know were to look */
  if (!mtype2)
    return -3;

  /* set the station name */
  string staname = stripstr (str);

  /* initialize remark to empty */
  remark = "";

  /* various variables */
  string line;
  std::vector<string> strvec;
  int i(-1);
  size_t pos;
  int status;

  /* preset status */
  status = 1;

  /* go the the TYPE 002 line */
  mbuf.seekg (mtype2,std::ios::beg);

  /* read first line */
  getline (mbuf,line);
  pos = mbuf.tellg ();

  /* information to collect */
  string name,old_name,igs_code;
  datetime::Date d1,d2;

  // keep on searching new lines until TYPE 003 is found or eof encountered
  while (pos<mtype3) {
    if (line.size()>50) {
      /* split the line to strings */
      strvec = splitstr (line);
      /* any valid line must hold at least STATION NAME,YYYY MM DD HH MM SS */
      if (strvec.size()>7) {
        name = strvec[0];
        /* try to match the station name */
        if (name==staname) {
          status = 2;
          /* if the name matches, try for vital information; 
          first get starting date (may or may not exist) */
          string d2str = line.substr (26,20);
          string tmp   = stripstr (d2str);
          if (!tmp.size()) {
            d1.setnow ();
            d1.addsec (-36400.0e0);
          } else {
            i = d1.fromymdstr (tmp);
            if (i) {
              #ifdef DEBUG
              std::cerr<<"\nERROR! Could not extract date from bern-sta file: "<<mfilename
              <<"\nERROR (1) resolving date from ["<<line.substr (26,20)<<"]"
              <<", line was ["<<line<<"]";
              #endif
              return -2;
            }
          }
          /* get stop date (if any) */
          d2str = line.substr (47,20);
          tmp = stripstr (d2str);
          /* if some string exists, try to resolve date */
          if (tmp.size()) {
            i = d2.fromymdstr (d2str);
            if (i) {
              #ifdef DEBUG
            std::cerr<<"\nERROR! Could not extract date from bern-sta file: "<<mfilename
            <<"\nERROR (2) resolving date from ["<<line.substr (47,20)<<"]"
            <<", line was ["<<line<<"]";
              #endif
              return -2;
            }
          /* if string is empty, set the stop date to tomorow */
          } else {
            d2.setnow ();
            d2.addsec (36400.0e0);
          }
          /* now check the time interval date */
          if ((d>=d1)&&(d<=d2)) {
            /* date ok; collect information */
            try {
              flag     = std::stoi (line.substr (22,3));
              receiver = line.substr ( 69,20);
              antenna  = line.substr (121,20);
              n        = (double) std::stof (line.substr (174,8));
              e        = (double) std::stof (line.substr (184,8));
              u        = (double) std::stof (line.substr (194,8));
              status   = 0;
            } catch (std::out_of_range&) {
              std::cerr<<"\nERROR! Could not extract station information from bern-sta file: "<<mfilename;
              std::cerr<<"\nLine is: ["<<line<<"]";
              return 4;
            } catch (std::invalid_argument) {
              std::cerr<<"\nERROR! Could not extract station information from bern-sta file: "<<mfilename;
              std::cerr<<"\nLine is: ["<<line<<"]";
              return 4;
            }
            if (line.size()>227) remark = line.substr (227);
            /* all done; we can return now */
            break;
          }
        }
      }
    }
    /* read next line */
    getline (mbuf,line);
    /* check for eof */
    if (mbuf.eof()) break;
    /* reset the position flag */
    pos = mbuf.tellg ();
  }

  /* return the status */
  return status;
}

/**
 * @details Gather information for a specific station for a specific epoch, from
 *          the section TYPE 003: HANDLING OF STATION PROBLEMS. The information
 *          include a flag and a remark.
 *
 * @param[in]  str      The name of the station (only name - code is dergarded) 
 *                      (\c std::string).
 * @param[in]  d        The epoch for which the information is requested 
 *                      (\c datetime::Date).
 * @param[out] flag     The flag of the station for this epoch (\c int).
 * @param[out] remark   The remark (comment) of the station for this epoch if any
 *                      (\c std::string).
 * @return              An integer deonoting the exit status, which can be:
 *                      Retuned Integer | Status
 *                      ----------------|---------------------
 *                                   -3 | TYPE 003: HANDLING OF STATION PROBLEMS not set (i.e. mtype3=0)
 *                                   -2 | invalid file format (unresolved date string)
 *                                   -1 | steam failure
 *                                    0 | all ok
 *                                    1 | station not found
 *                                    2 | station not found for this interval
 *                                    3 | -
 *                                    4 | could not collect info (i.e. invalid info line)
 *
 */
int Bernsta::gettype3flag (const std::string& str,const datetime::Date& d,int& flag,
                          string& remark)
{
  /* quick return if stream is invalid */
  if ((!streamopen) || (!mbuf.is_open ()))
    return -1;

  /* check that we know were to look */
  if (!mtype3)
    return -3;

  /* set the station name */
  string staname = stripstr (str);

  /* various variables */
  string line;
  std::vector<string> strvec;
  int i(-1);
  size_t pos;
  int status;

  /* preset status */
  status = 1;

  /* go the the TYPE 003 line */
  mbuf.seekg (mtype3,std::ios::beg);

  /* read first line */
  getline (mbuf,line);
  pos = mbuf.tellg ();

  /* information to collect */
  string name,old_name,igs_code;
  datetime::Date d1,d2;

  // keep on searching new lines until TYPE 004 is found or eof encountered
  while (pos<mtype4) {
    if (line.size()>20) {
      /* split the line to strings */
      strvec = splitstr (line);
      /* any valid line must hold at least STATION NAME,YYYY MM DD HH MM SS */
      if (strvec.size()>7) {
        name = strvec[0];
        /* try to match the station name */
        if (name==staname) {
          status = 2;
          /* if the name matches, try for vital information; 
          first get starting date (may or may not exist) */
          string d2str = line.substr (26,20);
          string tmp   = stripstr (d2str);
          if (!tmp.size()) {
            d1.setnow ();
            d1.addsec (-36400.0e0);
          } else {
            i = d1.fromymdstr (tmp);
            if (i) {
              #ifdef DEBUG
              std::cerr<<"\nERROR! Could not extract date from bern-sta file: "<<mfilename
              <<"\nERROR (1) resolving date from ["<<line.substr (26,20)<<"]"
              <<", line was ["<<line<<"]";
              #endif
              return -2;
            }
          }
          /* get stop date (if any) */
          d2str = line.substr (47,20);
          tmp = stripstr (d2str);
          /* if some string exists, try to resolve date */
          if (tmp.size()) {
            i = d2.fromymdstr (d2str);
            if (i) {
              //std::cerr<<"\nERROR! Could not extract date from bern-sta file: "<<mfilename;
              return -2;
            }
          /* if string is empty, set the stop date to tomorow */
          } else {
            d2.setnow ();
            d2.addsec (36400.0e0);
          }
          /* now check the time interval date */
          if ((d>=d1)&&(d<=d2)) {
            /* date ok; collect information */
            try {
              flag = std::stoi (line.substr (22,3));
              if (line.size ()>70) remark = line.substr (69);
              else remark = "";
              status   = 0;
            } catch (std::out_of_range&) {
              std::cerr<<"\nERROR! Could not extract station information from bern-sta file: "<<mfilename;
              std::cerr<<"\nLine is: ["<<line<<"]";
              return 4;
            } catch (std::invalid_argument) {
              std::cerr<<"\nERROR! Could not extract station information from bern-sta file: "<<mfilename;
              std::cerr<<"\nLine is: ["<<line<<"]";
              return 4;
            }
            /* all done; we can return now */
            break;
          }
        }
      }
    }
    /* read next line */
    getline (mbuf,line);
    /* check for eof */
    if (mbuf.eof()) break;
    /* reset the position flag */
    pos = mbuf.tellg ();
  }

  /* return the status */
  return status;
}
