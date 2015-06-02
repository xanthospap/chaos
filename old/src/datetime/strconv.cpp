#include "dates.hpp"

using namespace datetime;

/**
 * @details Transform an std::string to a valid datetime::TIME_SYSTEM enum.
 *          If the string does not match any of the enum types, then
 *          datetime::TIME_SYSTEM::Unknown is returned and the status is
 *          set accordingly.
 *
 * @param[in]  str    A string representing the time system (\c string).
 * @param[out] status The status of the transformation (\c integer).
 * @return            The time system represented by \c str or datetime::TIME_SYSTEM::Unknown
 *                    if no match is found (\c datetime::TIME_SYSTEM).
 *
 * @note    
 *  -# The \c status of the transformation can be:
 *     Status | Tranformation | time_system set
 *     -------|---------------|--------------------------------------------------
 *          0 | OK (success)  | time system set to the correct value
 *          1 | ERROR (failed)| time system set to datetime::TIME_SYSTEM::Unknown
 *
 *  -# The function is able to handle any \c str input parameters that are output
 *     values from the datetime::timesystem2string function.
 *
 */
TIME_SYSTEM datetime::string2timesystem (const std::string& str,int& status)
{
  status = 0;

  if (str=="Unknown")
    return TIME_SYSTEM::Unknown;
  else if (str=="UTC")
    return TIME_SYSTEM::UTC;
  else if (str=="UT1")
    return TIME_SYSTEM::UT1;
  else if (str=="TAI")
    return TIME_SYSTEM::TAI;
  else if (str=="TT")
    return TIME_SYSTEM::TT;
  else if (str=="GPS")
    return TIME_SYSTEM::GPS;
  else if (str=="GLO")
    return TIME_SYSTEM::GLO;
  else if (str=="GAL")
    return TIME_SYSTEM::GAL;
  else if (str=="BDT")
    return TIME_SYSTEM::BDT;
  else if (str=="QZS")
    return TIME_SYSTEM::QZS;
  else 
    status = 1;

  return TIME_SYSTEM::Unknown;
}

/**
 * @details Transform a datetime::TIME_SYSTEM enum to a string.
 *
 * @param[in]  ts  The TIME_SYSTEM enum to transform (\c datetime::TIME_SYSTEM).
 * @return         A string representing the time system \c ts (\c string).
 *
 * @note    Note that the strings returned by this function, match exactly the ones
 *          that can be used as input in the datetime::string2timesystem function.
 *
 */
std::string datetime::timesystem2string (const datetime::TIME_SYSTEM& ts)
{
  if (ts==TIME_SYSTEM::Unknown)
    return "Unknown";
  else if (ts==TIME_SYSTEM::UTC)
    return "UTC";
  else if (ts==TIME_SYSTEM::UT1)
    return "UT1";
  else if (ts==TIME_SYSTEM::TAI)
    return "TAI";
  else if (ts==TIME_SYSTEM::TT)
    return "TT";
  else if (ts==TIME_SYSTEM::GPS)
    return "GPS";
  else if (ts==TIME_SYSTEM::GLO)
    return "GLO";
  else if (ts==TIME_SYSTEM::GAL)
    return "GAL";
  else if (ts==TIME_SYSTEM::BDT)
    return "BDT";
  else if (ts==TIME_SYSTEM::QZS)
    return "QZS";
  else
    return "Unknown";
}
