#include "varutils.hpp"

/**
 * @details This function will strip an std::string of from ocurances of char 'c'
 *          both at the beginning and at the end (i.e. leading and trailing).
 *
 * @param[in] str The given string to clear (\c string).
 * @param[in] c   (optional) the character to erase from start and end; default
 *                value is ' ', i.e. whitespace (\c char).
 * @return        A string, cleared of from any occurancies of char c, either
 *                at the begining or at the end (\c string).
 *
 */
std::string stripstr (const std::string& str,const char c)
{
  // UPDATE OCT-2014 : new implementation via STL

  auto strBegin = str.find_first_not_of (" ");

  // if std::string::npos is returned, then this is an empty string or a string
  // full of whitespaces
  if (strBegin == std::string::npos) return "";

  auto strEnd   = str.find_last_not_of (" ");

  auto strRange = strEnd - strBegin + 1;

  // all done
  return str.substr (strBegin, strRange);

/*
  // ORIGINAL IMPLEMENTATION
  //////////////////////////////////////////////////////////////////////////////

  // copy input string
  std::string s (str);

  // quick return
  if (!s.size()) return s;

  // strip from begining
  while (*s.begin()==c) s.erase (s.begin());

  // strip from end
  while (*(s.end()-1)==c) s.erase (s.end()-1);

  // all done
  return s;
*/
}
