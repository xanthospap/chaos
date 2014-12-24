#include <stdexcept>
#include <iostream>
#include "obsrnx.hpp"
#include "varutils.hpp"

using std::string;
using std::cerr;

using namespace ngpt;

/**
 * @details This function will read the first line of the Rinex file, and try
 *          to resolve the RINEX version and verify the type (which must be 'O'
 *          i.e. observation RINEX). The function will only read the first
 *          line and nothing after that. On success, the member variable mversion
 *          will be set to a valid VERSION enumerator. No other information is
 *          extracted from the first line whatsoever. If the function fails, then
 *          the instance's version is set to VERSION::Unknown;
 *
 * @return  An integer denoting the function status; this can be:
 *          Returned Value | Status
 *          ---------------|---------------
 *                      -1 | Stream error
 *                       0 | OK
 *                       1 | Invalid line / format
 */
int ObsRinex::resolve_version ()
{
  // pre-set version to Unknown
  mversion = VERSION::Unknown;

  // make sure stream is open and in good state
  if ( this->bufstate() ) {
    cerr<<"\n[ERROR] Rinex stream failed! ";
    cerr<<"\n[ERROR] Cannot resolve version! for Observation Rinex file: "<< filename ();
    return -1;
  }

  // various variables
  string line,field,strend;

  // go to the begining of the file
  stream().seekg (0,std::ios::beg);

  // read in first line
  getline (stream(),line);

  // split line in column 60
  field = splitstr (line,strend,60);
  if (strend != "RINEX VERSION / TYPE") {
    cerr<<"\n[ERROR] Expected line: RINEX VERSION / TYPE, found ["<<line<<"]";
    cerr<<"\n[ERROR] Cannot resolve version! for Observation Rinex file: "<< filename ();
    return 1;
  }

  // resolve version
  float fvers;
  try { 
    fvers = std::stof ( field.substr (0,10) );
  } catch (std::invalid_argument&) {
    cerr<<"\n[ERROR] Cannot resolve Rinex version from line: ["<<line<<"]";
    cerr<<"\n[ERROR] Cannot resolve version! for Observation Rinex file: "<< filename ();
    return 1;
  }
  fvers *= 100;
  int iv = (int) (fvers+.5);
  if      (iv==100) mversion = VERSION::v100;
  else if (iv==200) mversion = VERSION::v200;
  else if (iv==210) mversion = VERSION::v210;
  else if (iv==211) mversion = VERSION::v211;
  else if (iv==300) mversion = VERSION::v300;
  else if (iv==301) mversion = VERSION::v301;
  else if (iv==302) mversion = VERSION::v302;
  else {
    cerr<<"\n[ERROR] Cannot match Rinex version from line: ["<<line<<"]";
    cerr<<"\n[ERROR] Cannot resolve version! for Observation Rinex file: "<< filename ();
    return 1;
  }

  // verify Rinex type; must be Observation, i.e. 'O'
  char type = line[20];
  if ( type != 'O' && type != 'o' ) {
    cerr<<"\n[ERROR] Expected Rinex type 'O', found : ["<<type<<"] at line 1";
    cerr<<"\n[ERROR] Cannot read header! for Observation Rinex file: "<< filename ();
    return 1;
  }

  // if we made it to here, everything is ok
  return 0;

}
