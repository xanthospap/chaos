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
 *          for a RINEX V1.xx. The header fields read and the variables set are 
 *          described in detail at the table below. The header section can not 
 *          contain more than _MAX_HEADER_LINES_ lines. On suceesseful exit, the
 *          function will end reading after the line with field 'END OF HEADER'
 *          has been matched, and mark this place as the mstartbit. That means,
 *          that the next line to be read must be a record header. On failure,
 *          the mstartbit is set to zero.
 *
 * @note    Note that RINEX v2.xx can only hold data for satellite systems: Gps,
 *          Glonass, Galileo, Sbas
 *
 * @warning 
 *        - The function ObsRinex::resolve_version should already have been run
 *          for the instant, to resolve the Version and Type. These fields are
 *          not checked/verified by this function.
 *        - Note that the observables in RINEX v2.xx (in header) are declared without
 *          any satellite system information. This means, that after resolving the
 *          (possible) observables, the mnumofobsv and the size of the mobsv vector
 *          may be different.
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
 * Header Field         | Considered ? | Variables set           | Comments
 * ---------------------|--------------|-------------------------|----------------
 * RINEX VERSION / TYPE | Yes          | msatsys                 | mversion and type already verified/set by ObsRinex::resolve_version
 * PGM / RUN BY / DATE  | No (skipped) | None                    | -
 * COMMENT              | No (skipped) | None                    | -
 * MARKER NAME          | Yes          | msite::mname,msite::mid | the name is set from the 20 first chars; the id from the first 4
 * MARKER NUMBER        | Yes          | msite::mnumber          | -
 * OBSERVER / AGENCY    | No (skipped) | None                    | -
 * REC # / TYPE / VERS  | Yes          | msite::mreceiver        | Only TYPE considered (not # and VERS)
 * ANT # / TYPE         | Yes          | msite::mantenna         | Sets both the antenna type and radome
 * APPROX POSITION XYZ  | Yes          | msite::mcrd             | -
 * ANTENNA: DELTA H/E/N | Yes          | msite::mdhen            | -
 * # / TYPES OF OBSERV  | Yes          | mnumofobsv,mobsv        | Note that for RINEX v < 3.00, the size of mobsv and mnumofobsv may differ
 * INTERVAL             | No           | None                    | -
 * TIME OF FIRST OBS    | Yes          | mtimesystem,mfirstdate  | If no time system reported, use the same as the satellite system (if not Mixed)
 * TIME OF LAST OBS     | Yes          | mlastdate               | -
 * RCV CLOCK OFFS APPL  | Yes          | mrcvclkappl             | -
 * LEAP SECONDS         | No           | None                    | -
 * # OF SATELLITES      | No           | None                    | -
 * PRN / # OF OBS       | No           | None                    | -
 * END OF HEADER        | Yes          | mstartbit               | Marks the end of reading
 *
 * Reference @cite rnx211
 */
int ObsRinex::readheader_v2x ()
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

  // this flag denotes that the default WAVELENGTH Factors are set
  bool default_wlf_set = false;

  // this flag denotes that a field '# / TYPES OF OBSERV' has alredy been read
  bool types_of_observ_set = false;

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

    else if (strend=="WAVELENGTH FACT L1/2") {
      bool exception_thrown = false;
      if ( ! default_wlf_set ) {                         // default wavelength factors
        try {
          ml12_wlf = new std::vector<int> (2,1);         // create the vector to hold the wavelength factors;
                                                         // at initialization both are 1.
          int factor = std::stoi ( field.substr (0,6) ); // resolve wlf for GPS L1
          ml12_wlf->operator[](0)=(factor);              // set GPS L1 wlf at the vector
          factor = std::stoi ( field.substr (0,6) );     // resolve wlf for GPS L2
          ml12_wlf->operator[](1)=(factor);              // set GPS L2 wlf at the vector
          if ( ml12_wlf->operator[](0) != 1 || ml12_wlf->operator[](0) != 1 ) {
            cerr<<"\n[WARNING] Sqaring type receiver encountered!";
            msquaring = true;                            // set the squaring flag to true
          } else {
            msquaring = false;                           // set the squaring flag to false
          }
        } catch (std::bad_alloc&) {
          exception_thrown = true;
        } catch (std::invalid_argument&) {
          exception_thrown = true;
        } catch (std::out_of_range&) {
          exception_thrown = true;
        }
        if ( exception_thrown ) {
          cerr<<"\n[ERROR] Cannot read Wavelength factors! line: ["<<line<<"]";
          cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
          return 1;
        }
      } else {                                      // satellite-specific wavelength factors
        if ( ! ml12_wlf ) {                         // check that default wavelength factors are set
          cerr<<"\n[ERROR] Satellite-Specific wavelength factors appear before default!";
          cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
          return 6;
        }
        cerr<<"\n[WARNING] Satellite-specific wavelength factors encountered!";
        try {
          msat_wlf = new td_satint;                       // create the vector to hold the wavelength factors per satellite;
          std::pair<int,int> tmppair;                     // temporary array for ease of use
          int f1 = std::stoi ( field.substr (0,6) );      // resolve wlf for GPS L1
          int f2 = std::stoi ( field.substr (0,6) );      // resolve wlf for GPS L2
          tmppair = std::make_pair (f1,f2);               // make a pair with the two factors
          int sats_follow = std::stoi(field.substr(12,6));// number of satellites to follow
          int sat_counter = 0;                            // count resolved satellites
          Satellite sat;                                  // temporary satellite
          for (int i=17;i<80;i+=3) {                      // loop through the substring containing the satellites
            if (sat_counter==sats_follow) break;          // stop if we read all satellites
            ok = sat.setfromstr (field.substr(i,3));      // resolve satellite
            if ( ok ) {                                   // satellite unresolved
              exception_thrown = true;
              break;
            }                                             // satellite resolved; push back the pair
            msat_wlf->push_back (std::pair<Satellite,std::pair<int,int> > (sat,tmppair));
          }
        } catch (std::bad_alloc&) {
          exception_thrown = true;
        } catch (std::invalid_argument&) {
          exception_thrown = true;
        } catch (std::out_of_range&) {
          exception_thrown = true;
        }
        if ( exception_thrown ) {
          cerr<<"\n[ERROR] Cannot read satellite-specific wavelength factors! line: ["<<line<<"]";
          cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
          return 1;
        }
      }
    } // WAVELENGTH FACT L1/2

    else if (strend=="# / TYPES OF OBSERV") {
      bool exception_thrown = false;
      if ( ! types_of_observ_set ) {                             // is this the first field '# / TYPES OF OBSERV' to be read ?
        try {
          mnumofobsv = std::stoi ( field.substr (0,6) );         // Number of different observation types
        } catch (std::invalid_argument&) {
          exception_thrown = true;
        } catch (std::out_of_range&) {
          exception_thrown = true;
        }
      }
      if ( exception_thrown ) {                                  // could not resolve number of observables !
        cerr<<"\n[ERROR] Cannot resolve number of observables! line: ["<<line<<"]";
        cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
        return 6;
      }
      vector<string> obsvec = splitstr (field.substr (6,54));    // Split the string containing the observables
      vector<raw_obs> ro_tmp;                                    // temporary vector of raw_obs
      vector<SatelliteSystem> ss_vec;                            // if this RINEX only holds a specific satellite system, add it to the vector;
                                                                 // else, the function raw_obs::resolve_v2x will make any possible observable using 
                                                                 // all sat. systems a RINEX v2.xx can hold
      if (msatsys != SatelliteSystem::Mixed) ss_vec.push_back (msatsys);
      for (auto& x : obsvec) {                                   // for every observable string
        ro_tmp.clear ();                                         // clear temp array
                                                                 // get a list of all observables this observables can represent,
        ro_tmp = (ss_vec.size()) ? raw_obs::resolve_v2x (x,&ss_vec) : raw_obs::resolve_v2x (x) ;
                                                                 // given the possible satellite systems
        if ( ! ro_tmp.size () ) {                                // check for empty list (i.e. error!)
          cerr<<"\n[ERROR] Cannot resolve observable : ["<<x<<"]";
          cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
          return 1;
        }
        for (auto& y : ro_tmp) mobsv.push_back( Observable (y) );// add entries of tmp array to RINEX's list of observables
      }
    } // # / TYPES OF OBSERV

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
