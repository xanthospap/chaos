#include "gsite.hpp"

using namespace ngpt;

/**
 * @details Construct an antenna instance from a given char array
 *          describing the antenna type and radome. The instance can hold
 *          at maximum 20 chars, so any larger input array will be
 *          truncated. Also note that any smaller input array will be
 *          truncated with whitespace characters up to a size of 20.
 *          be skipped. If the radome is empty, then it will be replaced
 *          by 'NONE'. Radome field is found at position [_RADOME_START_AT_,_RADOME_START_AT_+3]
 *          i.e., given the input array: 
 *          " TRM59800.00          |  Dorne Margolin with chokerings, Model 59800.00       |"
 *          the mtype array will hold the value " TRM59800.00      NONE"
 *
 * @param[in] c An array of chars, describing the antenna type plus radome (\c const char*).
 *  
 */
Antenna::Antenna (const char* c) 
{
  // copy input string
  size_t sz = std::strlen (c);
  sz>20 ? std::memcpy (mtype,c,20*sizeof(char)) : std::memcpy (mtype,c,sz*sizeof(char)) ;

  // pad with whitespaces if needed
  if (sz<20) while (sz<=20) mtype[sz++] = ' ';

  // last char must be a null-terminating char
  mtype[20] = '\0';
}

/**
 * @details Construct an antenna instance from a given a string
 *          describing the antenna type plus radome. The receiver type can hold
 *          at maximum 20 chars, so any larger input array will be
 *          truncated. Also note that any smaller input array will be
 *          truncated with whitespace characters up to a size of 20.
 *          If the radome is empty, then it will be replaced
 *          by 'NONE'. Radome field is found at position [_RADOME_START_AT_,_RADOME_START_AT_+3]
 *          i.e., given the input array:  
 *          " TRM59800.00          |  Dorne Margolin with chokerings, Model 59800.00       |"
 *          the mtype array will hold the value " TRM59800.00      NONE"
 *
 * @param[in] s An array of chars, describing the antenna type plus radome (\c string).
 *  
 */
Antenna::Antenna (const std::string& s) 
{      
  // copy the input string
  size_t sz = s.size ();
  if (sz>20) {
    std::copy (s.begin(),s.begin()+20,mtype);
  } else {
    std::copy (s.begin(),s.end(),mtype);
    while (sz<=20) mtype[sz++] = ' ';
  }

  // last char must be a null-terminating char
  mtype[20] = '\0';
}

/**
 * @details This function will inspect the mtype array and fix the radome. If
 *          entries [_RADOME_START_AT_,_RADOME_START_AT_+3] are empty (i.e. 
 *          whitespace), it will replace them with 'NONE'.
 *
 * @return A pointer to the first char of the radome, i.e. mtype+_RADOME_START_AT_ (char *)
 */
char* Antenna::fix_radome ()
{
  for (int i=0;i<_RADOME_START_AT_;i++) if ( *(mtype+i) == '\0' ) mtype[i] = ' ';

  bool replace = true;
  for (int i=_RADOME_START_AT_;i<20;i++) {
    if ( *(mtype+i) != ' ' ) {
      if ( *(mtype+i) == '\0' ) {
        *(mtype+i) = ' ';
      } else {
        replace = false;
      }
    }
  }

  if (replace) {
    mtype[_RADOME_START_AT_+0] = 'N';
    mtype[_RADOME_START_AT_+1] = 'O';
    mtype[_RADOME_START_AT_+2] = 'N';
    mtype[_RADOME_START_AT_+3] = 'E';
  }

  return mtype+_RADOME_START_AT_;
}
