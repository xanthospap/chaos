#include <stdexcept>
#include <iostream>
#include "obsrnx.hpp"
#include "varutils.hpp"

using std::string;
using std::vector;
using std::cerr;
using std::cout;

using namespace ngpt;

/*! maximum number of lines a header can have */
#define _MAX_HEADER_LINES_ 500

/**
 * @details Read and assign information contained in the RINEX's header section,
 *          for a RINEX V3.xx. The header fields read and the variables set are 
 *          described in detail at the table below. The header section can not 
 *          contain more than _MAX_HEADER_LINES_ lines. On suceesseful exit, the
 *          function will end reading after the line with field 'END OF HEADER'
 *          has been matched, and mark this place as the mstartbit. That means,
 *          that the next line to be read must be a record header. On failure,
 *          the mstartbit is set to zero.
 *
 * @warning The function ObsRinex::resolve_version should already have been run
 *          for the instant, to resolve the Version and Type. These fields are
 *          not checked/verified by this function.
 *
 * @return  An integer, denoting the function status, described below:
 *          Exit code | Status
 *         -----------|--------------------
 *                 -1 | Stream is bad
 *                  0 | Ok
 *                  1 | Failed to collect vital information
 *                  2 | Failed to find end of header
 *                  3 | Like 1, but discovered after header is read
 *                  4 | Found unknown field
 *                  6 | Some line is incorrect or misplaced
 *
 * @todo 
 *      - Field 'SYS / SCALE FACTOR' is not handled! Need more code ...
 *      - mscalefactor needs to be filled with all observables (default value is 1)
 *      - Field 'SYS / PHASE SHIFT' is not handled! Need more code ...
 *      - Field 'GLONASS SLOT / FRQ #' is not handled! Need more code ...
 *      - Filed 'GLONASS COD/PHS/BIS' is not handled! Need more code ...
 *
 * Header Field         | Considered ? | Variables set           | Comments
 * ---------------------|--------------|-------------------------|----------------
 * RINEX VERSION / TYPE | Yes          | msatsys                 | mversion and type already verified/set by ObsRinex::resolve_version
 * PGM / RUN BY / DATE  | No (skipped) | None                    | -
 * COMMENT              | No (skipped) | None                    | -
 * MARKER NAME          | Yes          | msite::mname,msite::mid | the name is set from the 20 first chars; the id from the first 4
 * MARKER NUMBER        | Yes          | msite::mnumber          | -
 * OBSERVER / AGENCY    | No (skipped) | None                    | -
 * MARKER TYPE          | Yes          | None                    | Only in Version > 3.xx; warning if site is not of type 'GEODETIC'
 * REC # / TYPE / VERS  | Yes          | msite::mreceiver        | Only TYPE considered (not # and VERS)
 * ANT # / TYPE         | Yes          | msite::mantenna         | Sets both the antenna type and radome
 * APPROX POSITION XYZ  | Yes          | msite::mcrd             | -
 * ANTENNA: DELTA H/E/N | Yes          | msite::mdhen            | -
 * ANTENNA: DELTA X/Y/Z | No           | None                    | Only in Version > 3.xx; Warning message
 * ANTENNA:PHASE CENTER | No           | None                    | Only in Version > 3.xx; Warning message
 * ANTENNA: B.SIGHT XYZ | No           | None                    | Only in Version > 3.xx; Warning message
 * ANTENNA: ZERODIR AZI | No           | None                    | Only in Version > 3.xx; Warning message
 * ANTENNA: ZERODIR XYZ | No           | None                    | Only in Version > 3.xx; Warning message
 * CENTER OF MASS: XYZ  | No           | None                    | Only in Version > 3.xx; Warning message
 * SYS / # / OBS TYPES  | Yes          | mobsv                   | -
 * SYS / PHASE SHIFT    | Yes          | None                    | If satellite or system dependent shifts exist, then error. Note that for RINEX v3.00 this field is not defines, for RINEX v3.01 it is named as SYS / PHASE SHIFTS and in RINEX v3.02 it is named SYS / PHASE SHIFT
 * SIGNAL STRENGTH UNIT | No (skipped) | None                    | -
 * INTERVAL             | No           | None                    | -
 * TIME OF FIRST OBS    | Yes          | mtimesystem,mfirstdate  | If no time system reported, use the same as the satellite system (if not Mixed)
 * TIME OF LAST OBS     | Yes          | mlastdate               | -
 * RCV CLOCK OFFS APPL  | Yes          | mrcvclkappl             | -
 * SYS / DCBS APPLIED   | Yes          | mdcbapplied             | -
 * SYS / PCVS APPLIED   | Yes          | mpcvapplied             | -
 * SYS / SCALE FACTOR   | Yes          | mscalefactor            | If any of the satellite systems has a factor other than 1, then all vailable satellite systems must be listed in the vector
 * SIGNAL STRENGTH UNIT | No (skipped) | None                    | Only in Version > 3.xx
 * LEAP SECONDS         | No (skipped) | None                    | -
 * # OF SATELLITES      | No (skipped) | None                    | -
 * PRN / # OF OBS       | No (skipped) | None                    | -
 * GLONASS COD/PHS/BIS  | No (skipped) | None                    | -
 * GLONASS SLOT / FRQ # | No (skipped) | None                    | -
 * END OF HEADER        | Yes          | mstartbit               | Marks the end of reading
 *
 * Reference @cite rnx302
 */
int ObsRinex::readheader_v3x ()
{
  // set the start of observations section to zero
  mstartbit = 0;

  // make sure stream is open and in good state
  if ( this->bufstate() ) {
    cerr<<"\n[ERROR] Rinex stream failed! Cannot read header";
    cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
    return -1;
  }

  // local copy of the buffer (ifstream)
  std::ifstream* buf = &stream ();

  // a dummy iterator to make sure that the reading loop doesn't go to infinity
  int dummy_iterator (0);

  // go to the begining of the file
  buf->seekg (0,std::ios::beg);

  // some basic variables to assist reading
  string line,strend,field;
  int ok;

  /* 
   * ---------------------------------------------------------------------------
   * First line must be: RINEX VERSION / TYPE. 
   * Check the observation system.
   * After this, the member variable msatsys should be set.
   * ------------------------------------------------------------------------ 
   */
  // read in first line
  getline (*buf,line);
  // split line in column 60
  field = splitstr (line,strend,60);
  if (strend != "RINEX VERSION / TYPE") {
    cerr<<"\n[ERROR] Expected line: RINEX VERSION / TYPE, found ["<<line<<"]";
    cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
    return 6;
  }
  // resolve satellite system
  char type = line[40];
  msatsys = c2satsys (type,ok);
  if ( ok ) {
    cerr<<"\n[ERROR] Cannot resolve Satelliet System from char : ["<<type<<"] at line 1";
    cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
    return 1;
  }

  /* 
   * ---------------------------------------------------------------------------
   * Every other field can be anywhere in the header. Keep reading
   * (header) lines until 'END OF HEADER' of EOF
   * ------------------------------------------------------------------------ 
   */
  // read in line
  getline (*buf,line);
  // read in, line-by-line
  while (true) {

    // check the iterator for max header lines
    if ( dummy_iterator++ > _MAX_HEADER_LINES_ ) {
      cerr<<"\n[ERROR] No 'END OF HEADER' found after "<<_MAX_HEADER_LINES_<<" lines; Header seems endless!";
      cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
      return 2;
    }

    // split line in column 60
    field = splitstr (line,strend,60);
    strend = stripstr (strend);

    // using the sub - string [60-80], try to match a valid RINEX header field

    if (strend=="MARKER NAME") {
      msite.setname ( field );
      msite.setidfromname ();
    } // MARKER NAME

    else if (strend=="MARKER NUMBER") {
      msite.setnumber ( field );
    } // MARKER NUMBER

    else if (strend=="REC # / TYPE / VERS") {
      msite.receiver () = field.substr (20,20);
    } // REC # / TYPE / VERS

    else if (strend=="ANT # / TYPE") {
      msite.antenna () = field.substr (20,20);
    } // ANT # / TYPE

    else if (strend=="PGM / RUN BY / DATE") {
      /**/
    } // PGM / RUN BY / DATE

    else if (strend=="OBSERVER / AGENCY") {
      /**/
    } // OBSERVER / AGENCY

    else if (strend=="INTERVAL") {
      /**/
    } // INTERVAL

    else if (strend=="SIGNAL STRENGTH UNIT") {
      /**/
    } // SIGNAL STRENGTH UNIT

    else if (strend=="LEAP SECONDS") {
      /**/
    } // LEAP SECONDS

    else if (strend=="# OF SATELLITES") {
      /**/
    } // # OF SATELLITES

    else if (strend=="PRN / # OF OBS") {
      /**/
    } // PRN / # OF OBS

    else if (strend=="GLONASS COD/PHS/BIS") {
      /**/
    } // COD/PHS/BIS

    else if (strend=="GLONASS SLOT / FRQ #") {
      /**/
    } // GLONASS SLOT / FRQ #

    else if (strend=="COMMENT") {
      /**/
    } // COMMENT

    else if (strend=="MARKER TYPE") {
      string type = line.substr (0,20);
      if ( stripstr (type) != "GEODETIC" ) {
        cerr<<"\n[WARNING] Site has MARKER TYPE other than GEODETIC! line: ["<<line<<"]";
        cerr<<"\n[WARNING] Filename: "<<filename();
      }
    } // MARKER TYPE

    else if (strend=="APPROX POSITION XYZ") {
      bool exception_thrown = false;
      try {
        msite.x () = std::stof ( field.substr ( 0,14) );
        msite.y () = std::stof ( field.substr (14,14) );
        msite.z () = std::stof ( field.substr (28,14) );
      } catch (std::invalid_argument&) {
        exception_thrown = true;
      } catch (std::out_of_range&) {
        exception_thrown = true;
      }
      if ( exception_thrown ) {
        cerr<<"\n[ERROR] Cannot read marker's approximate coordinates! line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
    } // APPROX POSITION XYZ

    else if (strend=="ANTENNA: DELTA H/E/N") {
      bool exception_thrown = false;
      try {
        msite.dh () = std::stof ( field.substr ( 0,14) );
        msite.de () = std::stof ( field.substr (14,14) );
        msite.dn () = std::stof ( field.substr (28,14) );
      } catch (std::invalid_argument&) {
        exception_thrown = true;
      } catch (std::out_of_range&) {
        exception_thrown = true;
      }
      if ( exception_thrown ) {
        cerr<<"\n[ERROR] Cannot read marker's eccentricities vector! line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
    } // ANTENNA: DELTA H/E/N

    else if (strend=="ANTENNA: DELTA X/Y/Z") {
      cerr<<"\n[WARNING] Found 'ANTENNA: DELTA X/Y/Z' for Observation Rinex file: "<< filename ();
    } // ANTENNA: DELTA X/Y/Z
    else if (strend=="ANTENNA:PHASE CENTER") {
      cerr<<"\n[WARNING] Found 'ANTENNA:PHASE CENTER' for Observation Rinex file: "<< filename ();
    } // ANTENNA:PHASE CENTER
    else if (strend=="ANTENNA: B.SIGHT XYZ") {
      cerr<<"\n[WARNING] Found 'ANTENNA: B.SIGHT XYZ' for Observation Rinex file: "<< filename ();
    } // ANTENNA: B.SIGHT XYZ
    else if (strend=="ANTENNA: ZERODIR AZI") {
      cerr<<"\n[WARNING] Found 'ANTENNA: ZERODIR AZI' for Observation Rinex file: "<< filename ();
    } // ANTENNA: ZERODIR AZI
    else if (strend=="ANTENNA: ZERODIR XYZ") {
      cerr<<"\n[WARNING] Found 'ANTENNA: ZERODIR XYZ' for Observation Rinex file: "<< filename ();
    } // ANTENNA: ZERODIR XYZ
    else if (strend=="CENTER OF MASS: XYZ") {
      cerr<<"\n[WARNING] Found 'CENTER OF MASS: XYZ' for Observation Rinex file: "<< filename ();
    } // CENTER OF MASS: XYZ

    else if (strend=="SYS / # / OBS TYPES") {
      char c = field[0];
      int cur_ss_obs_types = 0;
      // first resolve the satellite system and number of observables
      SatelliteSystem sstmp = c2satsys (c,ok);
      try {
        cur_ss_obs_types = std::stoi (line.substr (3,3));
      } catch (std::invalid_argument&) {
        ok = 1;
      } catch (std::out_of_range&) {
        ok = 1;
      }
      if ( ok ) {
        cerr<<"\n[ERROR] Cannot resolve RINEX's observables from line : ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
      // collect all observables to a new string
      string obs_str = field.substr (6,13*4);
      if ( cur_ss_obs_types > 13 ) {
        int lines_to_read = cur_ss_obs_types / (13+1);
        while ( lines_to_read ) {
          getline (*buf,line);
          obs_str += line.substr (6,13*4);
          --lines_to_read;
        }
      }
      // resolve the observables
      vector<string> vstr = splitstr (obs_str);
      if ( (int)vstr.size() != cur_ss_obs_types ) {
        cerr<<"\n[ERROR] Cannot resolve RINEX's observables from line : ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
      for (auto & x : vstr) {
        raw_obs to = raw_obs::resolve_v3x (x,sstmp,ok);
        if ( ok ) {
          cerr<<"\n[ERROR] Cannot resolve RINEX's observable : ["<<x<<"]";
          cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
          return 1;
        }
        mobsv.push_back ( Observable (to) );
      }
    } // SYS / # / OBS TYPES

    else if (strend=="TIME OF FIRST OBS") {
      // first try to resolve the time system
      this->timesystem() = datetime::string2timesystem (field.substr(48,3),ok);
      if ( ok ) { // error, except if the RINEX only holds a specific satellite system
        if      (msatsys==SatelliteSystem::Gps) timesystem() = datetime::TIME_SYSTEM::GPS;
        else if (msatsys==SatelliteSystem::Glonass) timesystem() = datetime::TIME_SYSTEM::GLO;
        else if (msatsys==SatelliteSystem::Galileo) timesystem() = datetime::TIME_SYSTEM::GAL;
        else {
          cerr<<"\n[ERROR] Cannot resolve RINEX's time system from line : ["<<line<<"]";
          cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
          return 1;
        }
      }
      // try to resolve the first epoch
      ok = firstdate().fromymdstr( field.substr(0,43),timesystem() );
      if ( ok ) {
        cerr<<"\n[ERROR] Cannot resolve time of first obs from line : ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
    } // TIME OF FIRST OBS

    else if (strend=="TIME OF LAST OBS") {
      // first try to resolve the time system
      datetime::TIME_SYSTEM ttmp = datetime::string2timesystem (field.substr(48,3),ok);
      if ( ok ) { // error, except if the RINEX only holds a specific satellite system
        if      (msatsys==SatelliteSystem::Gps) ttmp = datetime::TIME_SYSTEM::GPS;
        else if (msatsys==SatelliteSystem::Glonass) ttmp = datetime::TIME_SYSTEM::GLO;
        else if (msatsys==SatelliteSystem::Galileo) ttmp = datetime::TIME_SYSTEM::GAL;
        else {
          cerr<<"\n[ERROR] Cannot resolve RINEX's time system from line : ["<<line<<"]";
          cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
          return 1;
        }
      }
      // check that the first and last epochs are of the same time system
      if ( ttmp != timesystem() ) {
        cerr<<"\n[ERROR] First and last epochs belong in different time systems";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;      
      }
      // try to resolve the last epoch
      ok = lastdate().fromymdstr( field.substr(0,43),timesystem() );
      if ( ok ) {
        cerr<<"\n[ERROR] Cannot resolve time of last obs from line : ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
    } // TIME OF LAST OBS

    else if (strend=="RCV CLOCK OFFS APPL") {
      bool exception_thrown = false;
      try {
        mrcvclkappl = std::stoi ( field.substr (0,6) );
      } catch (std::invalid_argument&) {
        exception_thrown = true;
      } catch (std::out_of_range&) {
        exception_thrown = true;
      }
      if ( exception_thrown ) {
        cerr<<"\n[ERROR] Error resolving line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
    } // RCV CLOCK OFFS APPL

    else if (strend=="SYS / DCBS APPLIED") {
      char c = field[0];
      SatelliteSystem tss = c2satsys (c,ok);
      if ( ok ) {
        cerr<<"\n[ERROR] Error resolving satellite system at line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
      auto s = stripstr ( field.substr(20,40) );
      if ( s.size() ) mdcbapplied.push_back ( std::pair<SatelliteSystem,string> (tss,s) );
    } // SYS / DCBS APPLIED

    else if (strend=="SYS / PCVS APPLIED") {
      char c = field[0];
      SatelliteSystem tss = c2satsys (c,ok);
      if ( ok ) {
        cerr<<"\n[ERROR] Error resolving satellite system at line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
      auto s = stripstr ( field.substr(20,40) );
      if ( s.size() ) mpcvapplied.push_back ( std::pair<SatelliteSystem,string> (tss,s) );
    } // SYS / PCVS APPLIED

    else if (strend=="SYS / SCALE FACTOR") {
      cerr<<"\n[SHIT] Need more coding ...";
      char c = field[0];
      SatelliteSystem tss = c2satsys (c,ok);
      if ( ok ) {
        cerr<<"\n[ERROR] Error resolving satellite system at line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
      int factor;
      bool exception_thrown = false;
      try {
        factor = std::stoi (field.substr(2,4));
      } catch (std::invalid_argument&) {
        exception_thrown = true;
      } catch (std::out_of_range&) {
        exception_thrown = true;
      }
      if ( exception_thrown ) {
        cerr<<"\n[ERROR] Error resolving scale factor from line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
      auto s = stripstr ( field.substr(8,2) );
      if ( (s!= "  " && s != "01") && ( s != " 1") ) {
        cerr<<"\n[ERROR] Scale factors used per observable; line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
    } // SYS / SCALE FACTOR

    else if (strend=="SYS / PHASE SHIFTS" || strend=="SYS / PHASE SHIFT") {
      char c = field[0];
      SatelliteSystem tss = c2satsys (c,ok);
      if ( ok ) {
        cerr<<"\n[ERROR] Error resolving satellite system at line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
      // If no phase shifts, rest of the fields must be blank
      if ( (stripstr (field.substr(1,58))) != "" ) {
        cerr<<"\n[ERROR] Non-zero phase shifts for satellite system: ["<<c<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 1;
      }
    } // SYS / PHASE SHIFTS

    else if (strend=="END OF HEADER") {
      break;
    } // END OF HEADER

    else {
      std::cerr<<"\n[ERROR] Error resolving line: ["<<line<<"]";
      std::cerr<<"\n[ERROR] This line seems not to be explicitely mentioned in the RINEX specifications";
      std::cerr<<"\n[ERROR] or source code is missing to handle it!";
      return 4;
    } // UNKNOWN ??

    // read in next line
    getline (*buf,line);

    // check for eof
    if ( buf->eof () ) {
      cerr<<"\n[ERROR] EOF encountered before 'END OF HEADER' !";
      cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
      return 2;
    }

  } // while (true)

  // mark this bit (in the buffer) as the start of observations
  mstartbit = buf->tellg ();

  // if we reached this point, everything is ok! 
  return 0;
}
