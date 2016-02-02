#include "dates.hpp"
#include <string>
#include <string.h>

/**
 * @brief   Transform a date instant to a string of type: YYYY/MM/DD HH:MM::SS.SSS
 *
 * @details Write a date instant as an std::string of type: YYYY/MM/DD HH:MM::SS.SSS
 *
 * @param[in] status The status of the function, see Note 1 (\c int).
 * @param[in] ndp    (optional) The number of decimal places in the seconds field, and 
 *                   can have negative as well as positive values (\c int).
 *                   <b>The limits are platform dependent, but a safe range is -5 to +9.</b>
 *                   See Note #2 in the datetime::d2dtf function.
 * @return           A string holding the date, formated as: <br>
 *                   \c YYYY/MM/DD HH:MM::SS <br>
 *                   (\c string).
 *
 * @note 
 *  -# The status is actualy the status set by the call to function datetime::d2dtf. See the
 *     note on this function for return codes.
 *  -# Special care must be taken for leap seconds, if the time system of the date is UTC, or follows
 *     UTC (i.e. GLONASS time system). This is taken care of by the function datetime::d2dtf.
 *
 * @see  datetime::d2dtf
 *
 */
std::string datetime::Date::toymdstr(int& status,int ndp) const
{
  int iy,im,id,j;
  int hms[4];
  char scale[4] = "XXX";

  /* set status to ok */
  status = 0;

  /* if the time system is UTC, special care must be taken in dtf2d */
  if (mtsys==TIME_SYSTEM::UTC || mtsys==TIME_SYSTEM::GLO)
    strcpy (scale,"UTC");

  /* Format for output a 2-part Julian Date (or in the case of UTC a
  quasi-JD form that includes special provision for leap seconds). */
  j = datetime::d2dtf (scale,ndp,md1+DJM0,md2,iy,im,id,hms);

  /* check the exit status */
  if (j) {
    status = j;
    //std::cerr<<"\n*** assigned dubius or invalid date! (datetime::date::toymdstr)";
    //return "INVALID_DATE";
  }

  /* format the string (up to seconds) */
  std::string d = std::to_string (iy)+"/";
  d += (im>9)?(std::to_string (im)+"/"):("0"+std::to_string (im)+"/");
  d += (id>9)?(std::to_string (id)+"/"):("0"+std::to_string (id)+" ");
  d += (hms[0]>9)?(std::to_string (hms[0])+":"):("0"+std::to_string (hms[0])+":");
  d += (hms[1]>9)?(std::to_string (hms[1])+":"):("0"+std::to_string (hms[1])+":");
  d += (hms[2]>9)?(std::to_string (hms[2])+"."):("0"+std::to_string (hms[2])+".");

  /* format seconds to wanted precision */
  std::string dseconds = std::to_string (hms[3]);

  /* if the seconds have size less than ndp, the starting zeros are omited */
  if (ndp>1)
    if ((int) dseconds.size ()<ndp)
      while ((int) dseconds.size ()<ndp) 
        dseconds="0"+dseconds;

  /* add thes econds-string to the date-string */
  d += dseconds;

  /* return the string */
  return d;
}
