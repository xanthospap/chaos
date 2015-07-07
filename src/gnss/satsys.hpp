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

#include <cstddef>
#include <stdexcept>
#include <unordered_map>

namespace ngpt {

/// Enumeration for known Satellite systems. 
/// This is extracted from RINEX v3.02
enum class SATELLITE_SYSTEM : char {
    GPS,
    GLONASS,
    SBAS,
    GALILEO,
    BDS,
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
    /// Identifier
    static constexpr char identifier { 'G' };
    
    /// An assosiative array, containing [band,value] pairs for each frequency
    static constexpr std::unordered_map<short int,double> frequency_hash_map
    {{1,1575.42e0}, {2,1227.60e0}, {3,1176.45e0}};
    
    /// Return the nominal frequency, given a band
    static double nominal_frequency(short int i)
    {
        switch ( i ) {
            case (1) : return 1575.42e0;
            case (2) : return 1227.60e0;
            case (5) : return 1176.45e0;
            default  : throw std::runtime_error
            ("ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GPS>::"
             "nominal_frequency -> Invalid Band !!");
        }
    }
};

/// Specialize traits for Satellite System Glonass
template<>
struct SatelliteSystemTraits<SATELLITE_SYSTEM::GLONASS>
{
    /// Identifier
    static constexpr char identifier { 'R' };
    
    /// Number of valid (nominal) frequencies
    static constexpr std::size_t num_of_freqs { 3 };
    
    /// return the nominal frequency, given a band
    static double nominal_frequency(short int i)
    {
        switch ( i ) {
            case (1) : return 1602.000e0;
            case (2) : return 1246.000e0;
            case (3) : return 1202.025e0;
            default  : throw std::runtime_error
            ("ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GLONASS>::"
             "nominal_frequency -> Invalid Band !!");
        }
    }
};

/// Specialize traits for Satellite System Galileo
template<>
struct SatelliteSystemTraits<SATELLITE_SYSTEM::GALILEO>
{
    /// Identifier
    static constexpr char identifier { 'E' };
    
    /// Number of valid (nominal) frequencies
    static constexpr std::size_t num_of_freqs { 5 };
    
    /// return the nominal frequency, given a band
    static double nominal_frequency(short int i)
    {
        switch ( i ) {
            case (1) : return 1575.420e0; /// E1
            case (5) : return 1176.450e0; /// E5a
            case (7) : return 1207.140e0; /// E5b
            case (8) : return 1191.795e0; /// E5(E5a+E5b)
            case (6) : return 1278.750e0; /// E6
            default  : throw std::runtime_error
            ("ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GALILEO>::"
             "nominal_frequency -> Invalid Band !!");
        }
    }
};

/// Specialize traits for Satellite System SBAS
template<>
struct SatelliteSystemTraits<SATELLITE_SYSTEM::SBAS>
{
    /// Identifier
    static constexpr char identifier { 'S' };
    
    /// Number of valid (nominal) frequencies
    static constexpr std::size_t num_of_freqs { 2 };
    
    /// return the nominal frequency, given a band
    static double nominal_frequency(short int i)
    {
        switch ( i ) {
            case (1) : return 1575.42e0;
            case (5) : return 1176.45e0;
            default  : throw std::runtime_error
            ("ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::SBAS>::"
             "nominal_frequency -> Invalid Band !!");
        }
    }
};

/// Specialize traits for Satellite System QZSS
template<>
struct SatelliteSystemTraits<SATELLITE_SYSTEM::QZSS>
{
    /// Identifier
    static constexpr char identifier { 'J' };
    
    /// Number of valid (nominal) frequencies
    static constexpr std::size_t num_of_freqs { 4 };
    
    /// return the nominal frequency, given a band
    static double nominal_frequency(short int i)
    {
        switch ( i ) {
            case (1) : return 1575.42e0;
            case (2) : return 1227.60e0;
            case (5) : return 1176.45e0;
            case (6) : return 1278.75e0; /// LEX
            default  : throw std::runtime_error
            ("ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::QZSS>::"
             "nominal_frequency -> Invalid Band !!");
        }
    }
};

/// Specialize traits for Satellite System BDS
template<>
struct SatelliteSystemTraits<SATELLITE_SYSTEM::BDS>
{
    /// Identifier
    static constexpr char identifier { 'B' };
    
    /// Number of valid (nominal) frequencies
    static constexpr std::size_t num_of_freqs { 3 };
    
    /// return the nominal frequency, given a band
    static double nominal_frequency(short int i)
    {
        switch ( i ) {
            case (1) : return 1561.098e0;
            case (2) : return 1207.140e0;
            case (3) : return 1268.520e0;
            default  : throw std::runtime_error
            ("ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::BDS>::"
             "nominal_frequency -> Invalid Band !!");
        }
    }
};

/// Given a satellite system, return its identifier
char SatSysIdentifier(SATELLITE_SYSTEM);

/// Given a char (i.e. identifier), return the corresponding satellite system.
SATELLITE_SYSTEM charToSatSys(char);

} // end namespace
#endif
