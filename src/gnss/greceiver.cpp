#include "gsite.hpp"

using namespace ngpt;

/**
 * @details Construct a receiver instance from a given char array
 *          describing the receiver type. The receiver type can hold
 *          at maximum 20 chars, so any larger input array will be
 *          truncated. Also note that any smaller input array will be
 *          truncated with whitespace characters up to a size of 20.
 *          be skipped (i.e., given the input array: 
 *          " SEPT POLARX2C        |  L1/L2,code & carrier, all-in-view incl. 4 L2C ch."
 *          the mtype array will hold the value " SEPT POLARX2C        "
 *
 * @param[in] c An array of chars, describing the receiver type (\c const char*).
 *  
 */
Receiver::Receiver (const char* c) {

  // copy input string
  size_t sz = std::strlen (c);
  sz>20 ? std::memcpy (mtype,c,20*sizeof(char)) : std::memcpy (mtype,c,sz*sizeof(char)) ;

  // pad with whitespaces if needed
  if (sz<20) while (sz<=20) mtype[sz++] = ' ';

  // last char must be a null-terminating char
  mtype[20] = '\0';
}

/**
 * @details Construct a receiver instance from a given a string
 *          describing the receiver type. The receiver type can hold
 *          at maximum 20 chars, so any larger input array will be
 *          truncated. Also note that any smaller input array will be
 *          truncated with whitespace characters up to a size of 20.
 *          be skipped (i.e., given the input array: 
 *          " SEPT POLARX2C        |  L1/L2,code & carrier, all-in-view incl. 4 L2C ch."
 *          the mtype array will hold the value " SEPT POLARX2C        "
 *
 * @param[in] s A string, describing the receiver type (\c string).
 *  
 */
Receiver::Receiver (const std::string& s) {
      
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
