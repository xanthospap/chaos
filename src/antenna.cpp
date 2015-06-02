#include "antenna.hpp"

/** \details Construct an antenna instance from a given char array
 *          describing the antenna type (possibly including a radome type). 
 *          The antenna type can hold at maximum _MAX_ANTENNA_SIZE_ chars, so 
 *          any larger input array will be truncated. Also note that any 
 *          smaller input array will be truncated with whitespace characters up
 *          to a size of _MAX_ANTENNA_SIZE_.
 *          The input string is interpreted as:
 *          [ 0 - _ANTENNA_TYPE_SIZE_ )  the type of the antenna (e.g. 
 *          'AOAD/M_TA_NGS')
 *          [ _RADOME_OFFSET_ - _MAX_ANTENNA_SIZE_ ) the radome type (e.g. 
 *          'CONE')
 *
 * \param[in] c An array of chars, describing the antenna type plus radome (if 
 *              any)(\c const char*).
 *  
 */
ngpt::Antenna::Antenna (const char* c) {

  // copy input string
  size_t sz = std::strlen (c);
  sz > ngpt::_MAX_ANTENNA_SIZE_
      ? std::memcpy (name_,c,ngpt::_MAX_ANTENNA_SIZE_BYTES_) 
      : std::memcpy (name_,c,sz*sizeof(char)) ;

  // pad with whitespaces if needed
  if (sz < ngpt::_MAX_ANTENNA_SIZE_) 
      while (sz < ngpt::_MAX_ANTENNA_SIZE_) 
          name_[sz++] = ' ';
}

/** \details Construct an antenna instance from a given std::string
 *          describing the antenna type (possibly including a radome type). 
 *          The antenna type can hold at maximum _MAX_ANTENNA_SIZE_ chars, so 
 *          any larger input array will be truncated. Also note that any 
 *          smaller input array will be truncated with whitespace characters up
 *          to a size of _MAX_ANTENNA_SIZE_.
 *          The input string is interpreted as:
 *          [ 0 - _ANTENNA_TYPE_SIZE_ )  the type of the antenna (e.g. 
 *          'AOAD/M_TA_NGS')
 *          [ _RADOME_OFFSET_ - _MAX_ANTENNA_SIZE_ ) the radome type (e.g. 
 *          'CONE')
 *
 * \param[in] s A string, describing the antenna type plus radome (if any) 
 *             (\c std::string).
 *  
 */
ngpt::Antenna::Antenna (const std::string& s) {
      
  // copy the input string
  size_t sz = s.size ();
  if (sz > ngpt::_MAX_ANTENNA_SIZE_) {
    std::copy(s.begin(),s.begin()+ngpt::_MAX_ANTENNA_SIZE_,name_);
  } else {
    std::copy (s.begin(),s.end(),name_);
    while (sz < ngpt::_MAX_ANTENNA_SIZE_) 
        name_[sz++] = ' ';
  }
}
    
/// Set the Radome type
void ngpt::Antenna::setRadome(const char *c)
{
    std::size_t sz = std::strlen(c);

    sz > ngpt::_ANTENNA_RADOME_SIZE_
        ? std::memcpy(name_+_RADOME_OFFSET_,c,_ANTENNA_RADOME_SIZE_*sizeof(char))
        : std::memcpy(name_+_RADOME_OFFSET_,c,sz*sizeof(char)) ;

    // pad with whitespaces if needed
    if (sz < ngpt::_ANTENNA_RADOME_SIZE_) {
        while (sz < ngpt::_ANTENNA_RADOME_SIZE_) {
            name_[sz+ngpt::_RADOME_OFFSET_] = ' ';
            sz++;
        }
    }

    // all one
    return;
}

/// Set the Radome type
void ngpt::Antenna::setRadome(const std::string& s)
{
    // copy the input string
    std::size_t sz = s.size();
    
    if (sz > ngpt::_ANTENNA_RADOME_SIZE_) {
        std::copy(s.begin(),s.begin()+ngpt::_ANTENNA_RADOME_SIZE_,
                name_+ngpt::_RADOME_OFFSET_);
    } else {
        std::copy(s.begin(),s.end(),name_+ngpt::_RADOME_OFFSET_);
        while (sz < ngpt::_ANTENNA_RADOME_SIZE_) {
            name_[sz+ngpt::_RADOME_OFFSET_] = ' ';
            sz++;
        }
    }

    // all done
    return;
}