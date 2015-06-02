#include "receiver.hpp"

/** \details Construct a receiver instance from a given char array
 *          describing the receiver type. The receiver type can hold
 *          at maximum _RECEIVER_MAX_SIZE_ chars, so any larger input array 
 *          will be truncated. Also note that any smaller input array will be
 *          truncated with whitespace characters up to a size of 
 *          _RECEIVER_MAX_SIZE_. I.e., given the input array: 
 *          " SEPT POLARX2C        |  L1/L2,code & carrier, all-in-view incl. 4 L2C ch."
 *          the mtype array will hold the value " SEPT POLARX2C        "
 *
 * \param[in] c An array of chars, describing the receiver type (\c const char*).
 *  
 */
ngpt::Receiver::Receiver (const char* c) {

    // copy input string
    std::size_t sz = std::strlen (c);
    sz > ngpt::_RECEIVER_MAX_SIZE_ 
        ? std::memcpy (name_,c,ngpt::_RECEIVER_MAX_SIZE_BYTES_) 
        : std::memcpy (name_,c,sz*sizeof(char)) ;

    // pad with whitespaces if needed
    if (sz < ngpt::_RECEIVER_MAX_SIZE_) 
        while (sz < ngpt::_RECEIVER_MAX_SIZE_) 
            name_[sz++] = ' ';
}

/** \details Construct a receiver instance from a given a string
 *          describing the receiver type. The receiver type can hold
 *          at maximum _RECEIVER_MAX_SIZE_ chars, so any larger input array 
 *          will be truncated. Also note that any smaller input array will be
 *          truncated with whitespace characters up to a size of 
 *          _RECEIVER_MAX_SIZE_. I.e., given the input array: 
 *          " SEPT POLARX2C        |  L1/L2,code & carrier, all-in-view incl. 4 L2C ch."
 *          the mtype array will hold the value " SEPT POLARX2C        "
 *
 * \param[in] s A string, describing the receiver type (\c string).
 *  
 */
ngpt::Receiver::Receiver (const std::string& s) {
      
    // copy the input string
    std::size_t sz = s.size ();
    if (sz > ngpt::_RECEIVER_MAX_SIZE_) {
        std::copy (s.begin(),s.begin()+ngpt::_RECEIVER_MAX_SIZE_,name_);
    } else {
        std::copy (s.begin(),s.end(),name_);
        while (sz < ngpt::_RECEIVER_MAX_SIZE_) 
            name_[sz++] = ' ';
    }
}