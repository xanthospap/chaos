#include "dates.hpp"
#include <string.h>

/** 
 * @note  Note that the time system will be set to datetime::TIME_FORMAT::Unknown.
 */
datetime::Date::Date():md1(DJM00),md2(0.0e00),mtsys(datetime::TIME_SYSTEM::Unknown)
{
};

/** 
 * @param[in] d1 First part of a 2-part JD or MJD (\c double).
 * @param[in] d2 Second part of a 2-part JD or MJD (optional,\c double).
 * @param[in] tf (optional) The time format of the given date. Can be any of the
 *               datetime::TIME_FORMAT enums, though only JD and MJD
 *               will produce a valid date (\c datetime::TIME_FORMAT).
 * @param[in] ts (optional) The time system of the given date. If not provided, the 
 *               time system of the date will be set to datetime::TIME_SYSTEM::Unknown
 *               (\c datetime::TIME_SYSTEM). 
 *
 * The function will rearange (if needed) the two parts (\c md1 and \c md2),
 * so that the first part is an integer (i.e. MJ Day) and the second the fractional
 * part of the day.
 *
 * @note 
 * -# If the given date is not a Modified Julian date, then
 *  - if it is Julian Date, the constructor will convert it to MJD
 *  - else an invalid date will be set
 *
 */
datetime::Date::Date(double d1,double d2,TIME_FORMAT tf,TIME_SYSTEM ts):md1(d1),md2(d2),mtsys(ts)
{
  if (tf==TIME_FORMAT::JULIAN_DATE) {
    d1 -= DJM0;
  }
  
  this->rearange ();
};

/** 
 * @param[in] iy,im,id  Year, month, day (\c int).
 * @param[in] ihr,imn   Hour and minute (optional, \c int).
 * @param[in] sec       Seconds (optional, \c double).
 * @param[in] ts        The time system of the given date. If not provided, the 
 *                      time system of the date will be set to datetime::TIME_SYSTEM::Unknown
 *                      (optional, \c datetime::TIME_SYSTEM).
 * @note
 *  -# This constructor will call datetime::dtf2d to resolve the date 
 *     and transform it to a valid JD. Note however that this function may fail.
 *  -# If the time system (\c ts) is datetime::TIME_SYSTEM::UTC or 
 *     datetime::TIME_SYSTEM::GLO (which follows UTC), then special care
 *     must be taken for the leap seconds. This is taken care of by the
 *     call to datetime::dtf2d
 *  -# Via the call to datetime::dtf2d, it is always the case that the MJD will
 *     be split into 2 parts, where the first is an integer (MJDay) and the second is
 *     the fraction of day.
 *
 */
datetime::Date::Date(const int& iy,const int& im,const int& id,
      int ihr,int imn,double sec,TIME_SYSTEM ts)
{
  char scale[4] = "XXX";

  // if the time system is UTC, special care must be taken in datetime::dtf2d
  if (ts==TIME_SYSTEM::UTC||ts==TIME_SYSTEM::GLO)
    strcpy (scale,"UTC");

  // get the julian date
  /*int j = */datetime::dtf2d (scale,iy,im,id,ihr,imn,sec,md1,md2);

  // check that the exist status of dtf2d is ok
  // if (j) std::cerr<< "\n*** assigned dubius or invalid date! (datetime::date::date)";

  // time system as given
  mtsys = ts;
}
