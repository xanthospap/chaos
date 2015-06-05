#ifndef _SATELLITE_HPP_
#define _SATELLITE_HPP_

/**
 * \file      
 *
 * \version   
 *
 * \author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * \date      
 *
 * \brief     
 *
 * \details   
 *
 * \note      
 *
 * \todo      
 *
 * \copyright Copyright © 2015 Dionysos Satellite Observatory, <br>
 *            National Technical University of Athens. <br>
 *            This work is free. You can redistribute it and/or modify it under
 *            the terms of the Do What The Fuck You Want To Public License,
 *            Version 2, as published by Sam Hocevar. See http://www.wtfpl.net/
 *            for more details.
 *
 * <b><center><hr>
 * National Technical University of Athens <br>
 *      Dionysos Satellite Observatory     <br>
 *        Higher Geodesy Laboratory        <br>
 *      http://dionysos.survey.ntua.gr
 * <hr></center></b>
 *
 */

namespace ngpt {

/// Enumeration for known Satellite systems. 
/// This is extracted from RINEX v3.02
enum class SATELLITE_SYSTEM : char {
    GPS,
    GLONASS,
    SBAS,
    GALILEO,
    BEIDOU,
    QZSS,
    MIXED
};

/// Traits for Satellite Systems
template<SATELLITE_SYSTEM S>
struct SatelliteSystemTraits
{ };

/// Specialize traits for Satellite System Gps
template<>
struct SatelliteSystemTraits<SATELLITE_SYSTEM::GPS>
{
    /// Number of valid (nominal) frequencies
    static constexpr std::size_t num_of_freqs { 3 };
};

/// Enumeration for known Obsrvation Types. 
/// This is extracted from RINEX v3.02.
enum class OBSERVATION_TYPE : char {
    PSEUDORANGE,
    CARRIER_PHASE,
    DOPPLER,
    SIGNAL_STRENGTH,
    IONOSPHERE_PHASE_DELAY, /// see RINEX v3.02, sec. 5.12
    RECEIVER_CHANNEL_NUMBER /// see RINEX v3.02, sec. 5.13
};

/// Struct to hold observable attributes. See RINEX v3.02
struct Attribute 
{char c_;};


} // end namespace
#endif
