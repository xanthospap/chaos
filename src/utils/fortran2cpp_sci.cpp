#include <algorithm>
#include "varutils.hpp"

using std::string;

/*! Is this this char a 'd' or 'D' ? */
inline bool is_Dd (char c) {return ((c=='D')||(c=='d'));};

/**
 * @details This function converts a double precision floating point string from 
 *          fortran scientific format to c/c++ scientific format (e.g. \c 8934732D-5
 *          to \c 8934732E-5). Both 'D' and 'd' are handled. The returned string 
 *          will be stripped-of  (trimmed) from any leading and trailing whitespaces.
 *          This means that the input string can contain leading and/or trailing 
 *          whitespaces. The conversion will only be flaged as erroneous \c (ok=false) 
 *          if the  input string is empty. To further check the string, one must 
 *          enable the \c check input parameter.
 *          Enabling the \c check will make the function expilicitely check if
 *          the given input string is a FORTRAN floating point number.The check
 *          will fail, if any of the characters of the input string is not a digit
 *          AND is not one of <tt>'d' or 'D', '+' or '-', '.'</tt>. If any of these
 *          characters are encountered more than once, the check will fail except
 *          if it is a sign character (i.e. '+' and '-') which can can appear twice
 *          (e.g. -4.78d+6).
 *
 * @param[in] str   The input string to be converted (\c string).
 * @param[in] ok    A flag marking if the conversion was successeful (\c bool).
 * @param[in] check (optional) Perform a check to see if the input string \c str
 *                  is indeed a valid fortran formated floating point number;
 *                  default value is false (\c bool).
 * @return          If \c ok is true, the input string converted to c/c++ scientific 
 *                  format and trimmed of any leading or trailing whitespaces.
 *                  if \c ok is false, the conversion is ambiguous.
 *
 * @note The replacement could be done with a regular expression (gcc >= 4.9) since
 *       C++11. However, this version is faster.
 *
 */
string fortran2cpp_sci (const string& str, bool& ok, bool check)
{

  // default ok is false
  ok = false;

  // copy input string and trim whitespaces
  string s (stripstr(str));

  // quick return
  if (!s.size()) return s;

  if (!check) {
    // no checks performed; just replace D with E (upper & lower case)
    ok = true;
    std::replace_if(s.begin(), s.end(), is_Dd, 'E');
    /* THE REGEX WAY
     * ////////////////////////////////////////////////////////////////////////////
     * static std::regex reg2 ("d",std::regex_constants::icase);
     * return std::regex_replace (s,reg2,"e");
    */
  } else {
    // perform checks to see if the string is actually in scientific format
    ok = true;
    bool dot (false), d (false); // can only apear once
    int sign (0);                // can appear twice (e.g. -4.78d+6)
    for (auto &c : s) {
      if (!isdigit(c)) {
        if ((c=='d' || c=='D') && !d) {
          c='E';
          d=true;
        } else {
          if ((c=='-' || c=='+') && sign<2) {
            sign++;
          } else if (c=='.' && !dot) {
            dot = true;
          } else {
            ok = false;
            return s;
          }
        }
      }
    }
  }

  // all done
  return s;
}
