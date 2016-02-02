#include <vector>
#include <string>
#include <string.h>
#include <stdexcept>
#include "dates.hpp"
#include "varutils.hpp"

/**
 * @details Read and set a date from an std::string representing a Calendar date,
 *          formated in one of the following ways: <br>
 *          YYYY MM DD HH MM SS.SSS <br>
 *            YY MM DD HH MM SS.SSS
 *
 * @param[in]  str  The string representing the date (\c string).
 * @param[in]  ts   (optional) The time system of the date to be resolved. If not given,
 *                  datetime::TIME_SYSTEM::Unknown is assumed (\c datetime::TIME_SYSTEM)
 *                  (Note 5).
 * @return          An \c integer denoting the exit status of the function, which can be:
 *                  Return Value | Status
 *                  -------------|---------------
 *                  +3           | Both of next two
 *                  +2           | Time is after end of day (Note 1)
 *                  +1           | Dubious year (Note 2)
 *                   0           | OK
 *                  -1           | Bad year
 *                  -2           | Bad month
 *                  -3           | Bad day
 *                  -4           | Bad hour
 *                  -5           | Bad minute
 *                  -6           | Bad second (<0)
 *                  -7           | Error; bad string (Note 3)
 *
 * @note
 *   -# The warning status "time is after end of day" usually means that
 *      the sec argument is greater than 60.0.  However, in a day ending
 *      in a leap second the limit changes to 61.0 (or 59.0 in the case
 *      of a negative leap second).
 *  -# The warning status "dubious year" flags UTCs that predate the
 *      introduction of the time scale or that are too far in the future
 *      to be trusted. See datetime::dat for further details.
 *  -# This error code is general and can be caused by an ill-formated input
 *      string \c str.
 *  -# Special care is taken if the date given is in a type system dependent on
 *      UTC (i.e. UTC or GLONASS) because of the leap seconds. The function
 *      datetime::dtf2d takes care of this.
 *  -# In case the exit status is -7, the time system is not set.
 *
 * @see datetime::dtf2d
 */
int datetime::Date::fromymdstr(const std::string& str ,TIME_SYSTEM ts)
{
  int iy,im,id,ihr,imn,j=0;
  double sec (0.0e00);
  char scale[4] = "XXX";

  /* split the vector using whitespace as delimeter */
  std::vector<std::string> strvec = splitstr (str);

  /* six different string should be formed and assigned */
  if (strvec.size ()!=6) {
    //std::cerr<<"\nCannot resolve date from string: "+str;
    return -7;
  }

  /* special care if year is 2-digit */
  if (strvec[0].size ()==2) {
    try {
      int year = std::stoi (strvec[0]);
      iy = datetime::tdy2fdy (year);
    } catch (const std::invalid_argument& ia) {
      //std::cerr<<"\nInvalid argument: "<<ia.what()<<"\n";
      //std::cerr<<"\nCannot resolve date from string: "+str;
      return -7;
    } catch (const std::out_of_range& ofr) {
      //std::cerr<<"\nOut of range: "<<ofr.what()<<"\n";
      //std::cerr<<"\nCannot resolve date from string: "+str;
      return -7;
    }
  }
  else {
    iy = std::stoi (strvec[0]);
  }
  
  /* assign the im, id, ihr, imn, sec variables (string to numeric) */
  try {
    im  = std::stoi (strvec[1]);
    id  = std::stoi (strvec[2]);
    ihr = std::stoi (strvec[3]);
    imn = std::stoi (strvec[4]);
    sec = std::stod (strvec[5]);
  } catch (const std::invalid_argument& ia) {
    //std::cerr<<"\nInvalid argument: "<<ia.what()<<"\n";
    //std::cerr<<"\nCannot resolve date from string: "+str;
    return -7;
  } catch (const std::out_of_range& ofr) {
    //std::cerr<<"\nOut of range: "<<ofr.what()<<"\n";
    //std::cerr<<"\nCannot resolve date from string: "+str;
    return -7;
  }

  /* if the time system is UTC, special care must be taken in dtf2d */
  if (ts==TIME_SYSTEM::UTC || ts==TIME_SYSTEM::GLO)
    strcpy (scale,"UTC");

  /* get the julian date */
  j = datetime::dtf2d (scale,iy,im,id,ihr,imn,sec,md1,md2);

  /* check that the exist status of dtf2d is ok */
  //if (j) {
  //  std::cerr<<"\n*** assigned dubius or invalid date!";
  //  return j;
  //}

  /* set system time */
  mtsys = ts;

  /* return the status */
  return j;
}
