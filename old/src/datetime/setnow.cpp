#include "dates.hpp"
#include <stdio.h>
#include <time.h>

/** 
 * @brief Get current date/time as a char[]. Format is YYYY-MM-DD.HH:mm:ss
 *
 * @note This function is used by datetime::date::setnow to set a date
 *       to current epoch.
 *
 */
const std::string currentDateTime() {
  time_t     now = time (0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime (&now);

  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime (buf,sizeof(buf),"%Y %m %d %H %M %S",&tstruct);

  return buf;
}

/** 
 * @details  This function will (overwite) set a date instant to 
 *           represent the current epoch (date plus time). This is done
 *           by using the datetime::date::fromymdstr function on a
 *           string representing now, set by currentDateTime function.
 *
 * @return   \c integer denoting the exit status as shown in the following table
 *           (this is actually the exit status from the datetime::date::fromymdstr function)
 */
int datetime::Date::setnow ()
{
  return this->fromymdstr (currentDateTime ());
}
