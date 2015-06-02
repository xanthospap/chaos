#include <stdexcept>
#include "gsats.hpp"

using std::string;
using namespace ngpt;

/**
 * @details resolve Satellite from a RINEX string; this function can handle all
 *          RINEX versions.
 *
 * @see Satellite::setfromstr
 *
 */
Satellite::Satellite (const string& str)
  :msatsys(SatelliteSystem::Unknown),mprn(_SAT_UNDF_),msvn(_SAT_UNDF_),mmodel(SatelliteBlockModel::Unknown)
{
  this->setfromstr (str);
}

/**
 * @details Set a satellite given a string of type:<br>
 * case (1) [Sat.Sys][PRN] e.g. [G09] <br>
 * case (2) [Sat.Sys][whitespace][PRN<10] e.g. [G 9] <br>
 * case (3) [PRN] e.g. [ 09] or [9] <br>
 * In case (3), it is supposed that the satellite system is GPS. Any leading or
 * trailing spaces will be stripped off (so the length of the string does not
 * matter). If the function fails, the prn and svn will be set to -999, the 
 * satellite system and satellite block model to Unknown.
 *
 * @param[in] satstr the string describing the satellite (\c string).
 * @return           the status of the function (\c int) which can be:
 * Returned Int | Status
 * -------------|----------
 *            0 | success
 *            1 | failure
 *
 * Reference @cite rnx211 , @cite rnx302
 *
 */
int Satellite::setfromstr (const string& satstr)
{
  /* set to undefined */
  this->initialize ();

  /* remove any leading and/or trailing whitespaces */
  string str (stripstr (satstr));

  size_t size = str.size();

  /* pre-set status to error */
  bool status = 1;

  /* depending os the string's size, we have the following possibilities */
  switch (size) {

    /* size = 1; that should mean we address case 3;
    do not break; move on and treat it as it had a size of 2 */
    case 1:

    /* size = 2; that should mean we address case 3 */
    case 2:
      /* satellite system is GPS */
      msatsys = SatelliteSystem::Gps;
      /* try to resolve prn and set status */
      try {
        mprn = std::stoi(str);
        status = 0;
      } catch (std::invalid_argument) {
        status = 1;
      }
      break;

    /* size = 3; that should mean we address case 1 or 2 */
    case 3:
      int ok;
      /* try to resolve satellite system */
      msatsys = c2satsys (str[0],ok);
      /* check if the conversion failed */
      if (ok>0) { /* Failed set status to false */
        status = 1;
      } else { /* Satellite system ok; try for prn */
        try {
          mprn = std::stoi (str.substr(1,2));
          status = 0;
        } catch (std::invalid_argument) {
          status = 1;
        }
      }
      break;

    /* size > 3. That can't be right! */
    default:
      status = false;
      break;
  }

  /* if status is false, set to default values */
  if (status) this->initialize ();

  /* return the status */
  return status;
}
