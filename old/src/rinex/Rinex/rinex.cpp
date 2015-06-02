#include "rinex.hpp"

using std::string;
using namespace ngpt;

/**
 * @details Check if a line contained in a rinex file is a comment line.
 *          For this to be  true, columns 60-80 are checked and
 *          compared to "COMMENT" string. A perfect match is assumed
 *          to be a comment line (striping leading/rtailing whitespaces).
 *
 * @param[in] str the string to be tested for beeing a comment line (\c string).
 * @return        true or false; true meaning the input string is a comment line
 *
 */
bool Rinex::is_rinex_commentline (const string& str)
{
  string start,end;

  /* split the string in two parts, at column 80 */
  start = splitstr (str,end,80);

  /* strip any leading/trailing whitespaces */
  end = stripstr (end);

  /* split line as in all ibuf files */
  if (end=="COMMENT") return true;
  else return false;
}
