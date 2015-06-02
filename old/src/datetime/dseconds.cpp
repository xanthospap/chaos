#include "dates.hpp"

/**
 * @details Add \c sec seconds to a datetime instance. The seconds are added
 *          to the fractional part of the instant (i.e. \c md2) and a check is
 *          performed to check that \c md2 has no integer part. In case it does,
 *          the instant's \c md1 and \c md2 are re-aranged, so that \c md1 is
 *          the integer part (i.e. Modified Julian Day) and \c md2 is the fractional
 *          part.
 *
 * @param[in] sec Seconds to add (\c double).
 */
void datetime::Date::addsec (const double& sec)
{
  md2 += (sec / DAYSEC);

  while (md2>=1.0e0) {
    md2 -= 1.0e0;
    md1 += 1.0e0;
  }

  while (md2<=-1.0e0) {
    md2 += 1.0e0;
    md1 -= 1.0e0;
  }

  return;
}
