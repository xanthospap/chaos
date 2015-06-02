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

/// A struct to hold pairs of whatever ...
template<typename T1,typename T2>
struct _dummy_map_ 
{T1 index; T2 value;};

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

    /// Look-up table, holding pairs of [frequency_index, frequency_value]
    static constexpr _dummy_map_<short int,double> lut [] = {
        {1, 1575.420e0},
        {2, 1227.600e0},
        {5, 1176.450e0}
    };

    /// Get the nominal frequency value, given the frequency index
    static constexpr double nominal_frequency(short int f) {};
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

} // end namespace
#endif
