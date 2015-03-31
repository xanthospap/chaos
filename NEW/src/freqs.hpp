#ifndef __FREQUENCIES_SYSTEMS__
#define __FREQUENCIES_SYSTEMS__

# include "satsys.hpp"

namespace ngpt {

/** @brief Generalized template class for satellite system information
 */
template<SAT_SYS E, short int I>
inline constexpr double freq ()
{ return .0e0; }

/** @brief Specialized template class for Gps satellite system,  band L1
 */
template<>
inline constexpr double freq<SAT_SYS::GPS, 1> ()
{ return 1575.42e0; }

/** @brief Specialized template class for Gps satellite system,  band L2
 */
template<>
inline constexpr double freq<SAT_SYS::GPS, 2> ()
{ return 1227.60e0; }

/** @brief Specialized template class for Gps satellite system,  band L5
 */
template<>
inline constexpr double freq<SAT_SYS::GPS, 5> ()
{ return 1176.45e0; }

} // end namespace ngpt

#endif
