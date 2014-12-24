#include <algorithm>
#include <cmath>
#include "gobst.hpp"

using namespace ngpt;
using std::string;

/**
 * @details Resolve an observable type, from a string formated as in Rinex V3.x 
 *          Each observable is uniquely defined by its satellite system, the type,
 *          the carrier band and the attribute. Rinex v3.x observable strings
 *          have the general format: tna where t=type,n=band,a=attribute.
 *          The input string is cleared off from any leading/trailing whitespaces.
 *          Note that In RINEX v3.xx, the attribute can be left blank if not known. 
 *          So a whitespace char (as the third char in input string) is mapped to 
 *          TrackingCode::Unknown.
 *
 * @param[in]  cstr   the string to be resolved, holding a RINEX v3.x observable 
 *                    identifier (e.g. 'L1C') (\c string).
 * @param[in]  ss     the satellite system this observable belongs to (\c SatelliteSystem).
 * @param[out] status the status of the resolvation; if it is set to zero then the
 *                    function ended succesefuly, else the status is set to something
 *                    other than zero (\c int).
 * @return            if the status is zero, then the function returns the observable
 *                    described by the input string (i.e. cstr); if the status
 *                    is not zero then all members of the returned observable are
 *                    set to unknown (\c raw_obs).
 *
 * The exist code can be -1, 0, or >0. In case it is > 0, then it is a binary number
 * formed by the powers of two as described below (e.g. if the status is set to 8,
 * that means that the type and attribute failed to be resolved).
 * Exit Code | Status
 *-----------|----------------------------------------
 *        -1 | input string has size other than 3
 *         0 | success
 *      2**0 | unknown satellite system
 *      2**1 | unknown observation type
 *      2**2 | unknown carrier band
 *      2**3 | unknown tracking code / attribute
 *
 * Reference @cite rnx302
 *
 */
raw_obs raw_obs::resolve_v3x (const string& cstr,const SatelliteSystem& ss,
                            int& status)
{
  /* create and set instance to unknown */
  raw_obs rtn;
  rtn.setunknown ();

  /* remove any trailing or leading whitespaces */
  string str = stripstr (cstr);

  /* pre-set status to ok */
  status = 0;
  int j,n;

  /* the size of the string must be equal to 2 or 3; else exit 
  the only valid 2-digit obsrvation string is when the attribute is
  blank */
  if (str.size()!=3 && str.size()!=2) {
    status = -1;
    return rtn;
  }

  /* we have to know the satellite system */
  if (ss==SatelliteSystem::Unknown) {
    status = pow (2,0);
    return rtn;
  }

  /* first of all, assign the satellite system */
  rtn.ms = ss;

  /* assign the type */
  rtn.mt = c2obstype (str[0],j);
  if (j) status += pow (2,1);

  /* assign the carrier band */
  if ( (n=isdigit (str[1])) ) {
    rtn.mn = int2carrierband (rtn.ms,n,j);
    if (j) status += pow (2,2);
  } else {
    status += pow (2,2);
  }

  /* assign the attribute */
  if (str.size()==2) {
    rtn.ma = TrackingCode::Unknown;
  } else {
    rtn.ma = c2trackingcode (str[2],j);
    if (j) status += pow (2,3);
  }

  /* return the instance */
  return rtn;
}
