#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "varutils.hpp"

using std::vector;
using std::string;

//! for ease of use
typedef std::vector<std::string> strvec;

/**
 * @details This function will split a string at every ocurance of char \c d  
 *          and return the individual parts as a vector of strings.
 *
 * @param[in] str   The string to be split into parts (\c string).
 * @param[in] delim (optional) The delimeter; can be any char, default is 
 *                  whitespace (\c char).
 * @return          A vector of strings (\c std::vector<string>). Note that
 *                  the vector can be of zero size.
 *
 */
strvec splitstr (const string& str,const char delim)
{

  // empty array of strings
  strvec tokens;

  // look for quick return
  if (!str.size())
    return tokens;

  // copy the string to a string stream for further use
  std::istringstream iss (str);

  // delim = space; this is an easy case
  if (delim==' ') {
    std::copy(std::istream_iterator<string>(iss),
        std::istream_iterator<string>(),
        std::back_inserter<vector<string> >(tokens));

  // delim other than space
  } else {
    size_t pos (0),strsize (str.size());
    string token;
    while (str[pos]==delim) {
      pos++;
      if (pos==strsize)
        return tokens;
    }
    for (size_t i=pos;i<strsize;i++) {
      if (str[i]!=delim) {
        token += str[i];
      } else {
        if (token.size()) {
          tokens.push_back (token);
          token.clear();
        }
      }
    }
    if (token.size())
      tokens.push_back (token);
  }

  // all done
  return tokens;
}


/**
 * @brief   Split a string at the column number given and return the two parts.
 *
 * @details This function will split a string at the d<sup>th</sup> column.
 *          If the size of the string given is \c N, then the string returned will
 *          be the substring [0,d) and the string \c end will be the substring
 *          [d,N). If <tt>N = 0</tt>, then both the returned and the \c end strings
 *          are going to be empty. If N<d, then the returned string is the substring
 *          [0,N) and the string \c end is a null string. If <tt>d < 0</tt>, then
 *          both the returned and the \c end strings are null (empty).
 *
 * @param[in]  str  The string to be split into parts (\c string).
 * @param[out] end  The 2<sup>nd</sup> part of the split string (\c string),
 *                  i.e. the substring of \c str spanning [d,str.size()). Empty
 *                  if <tt>str.size() < d</tt>.
 * @param[in]  d    (optional) The column number at wich the \c str string is to
 *                  be split (\c int); default is d = 80.
 *
 */
string splitstr (const string& str, string& end, int d)
{
  
  // clear the string to be returned
  end.clear();

  // string to be returned
  string start;

  // if d < 0 return null strings
  if (d<0) return start;

  // get the size of the string
  int size = (int) str.size();

  // if size=0 just return an empty string
  if (!size) return start;

  // if size is smaller than d, copy the string to start and return it
  if (size<d) return str.substr(0,d);

  // else, split the string to start and end strings
  start = str.substr (0,d);
  end   = str.substr(d,size-d);

  // return the start part
  return start;
}
