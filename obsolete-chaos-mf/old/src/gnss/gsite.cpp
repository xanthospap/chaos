#include "gsite.hpp"

/**
 * @details Set the name (i.e. mname from an input string. The input string can
 *          have any size, but only the first 20 chars are going to be considered.
 *          If the input string has length < 20, then all remaining chars will be
 *          set to whitespace.
 *
 * @param[in] s A string, describing the site's name (\c string).
 *  
 */
void ngpt::Site::setname (const std::string& s) {

  // copy the input string
  size_t sz = s.size ();
  if (sz>20) {
    std::copy (s.begin(),s.begin()+20,mname);
  } else {
    std::copy (s.begin(),s.end(),mname);
    while (sz<=20) mname[sz++] = ' ';
  }

  // last char must be a null-terminating char
  mname[20] = '\0';
}

/**
 * @details Set the number (i.e. mnumber from an input string. The input string can
 *          have any size, but only the first 20 chars are going to be considered.
 *          If the input string has length < 20, then all remaining chars will be
 *          set to whitespace.
 *
 * @param[in] s A string, describing the site's number (\c string).
 *  
 */
void ngpt::Site::setnumber (const std::string& s) {

  // copy the input string
  size_t sz = s.size ();
  if (sz>20) {
    std::copy (s.begin(),s.begin()+20,mnumber);
  } else {
    std::copy (s.begin(),s.end(),mnumber);
    while (sz<=20) mnumber[sz++] = ' ';
  }

  // last char must be a null-terminating char
  mnumber[20] = '\0';
}
