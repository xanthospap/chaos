#include "dates.hpp"

/**
 * @details Rearange the two parts (i.e. \c md1 and \c md2) of a date (Modified 
 *          Julian Date) so that the first is an integer (i.e. Modified Julian
 *          Day) and the second is a decimal < 1.0 (i.e. the fraction of day).
 *
 * @todo    This function is incomplete and unchecked.
 */
void datetime::Date::rearange()
{
  /* Separate day and fraction. */
  double f1 (fmod (md1, 1.0e0));
  double f2 (fmod (md2, 1.0e0));
  double f  (fmod (f1+f2, 1.0e0));
  if (f < 0.0e0) f += 1.0e0;
  double d = floor(md1-f1)+floor(md2-f2)+floor(f1+f2-f);
  long jd = (long) floor(d);

  /* assign to members */
  md2 = f;
  md1 = (double) jd;

  /* all done */
  return;
}
