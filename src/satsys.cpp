#include "satsys.hpp"
#include <stdexcept>

/** \details  Given a satellite system enumerator, this function will return
 *            it's identifier (e.g. given SatelliteSystem = GPS, the function
 *            will return 'G'). The identifier are taken from RINEX v3.02
 * 
 *  \param[in] s Input Satellite System.
 *  \return      A char, representing the satellite system
 * 
 *  \throw       std::runtime_error if no matching satellite system is found.
 */
char ngpt::SatSysIdentifier(ngpt::SATELLITE_SYSTEM s)
{
    using ngpt::SATELLITE_SYSTEM;
    
    switch ( s ) {
        case SATELLITE_SYSTEM::GPS :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GPS>::identifier;
        case SATELLITE_SYSTEM::GLONASS :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GLONASS>::identifier;
        case SATELLITE_SYSTEM::SBAS :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::SBAS>::identifier;
        case SATELLITE_SYSTEM::GALILEO :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GALILEO>::identifier;
        case SATELLITE_SYSTEM::BDS :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::BDS>::identifier;
        case SATELLITE_SYSTEM::QZSS :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::QZSS>::identifier;
        default:
            throw std::runtime_error
            ("ngpt::SatSysIdentifier -> Invalid Satellite System !!");
    }
}

/** \details  Given a char, this function will return the corresponding
 *            satellite system. The identifier are taken from RINEX v3.02
 * 
 *  \param[in] c Input char.
 *  \return      The corresponding satellite system
 * 
 *  \throw       std::runtime_error if no matching satellite system is found.
 */
ngpt::SATELLITE_SYSTEM ngpt::charToSatSys(char c)
{
    using ngpt::SATELLITE_SYSTEM;
    
    switch ( c ) {
        case (ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GPS>::identifier) :
            return SATELLITE_SYSTEM::GPS;
        case (ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GLONASS>::identifier) :
            return SATELLITE_SYSTEM::GLONASS;
        case (ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GALILEO>::identifier) :
            return SATELLITE_SYSTEM::GALILEO;
        case (ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::SBAS>::identifier) :
            return SATELLITE_SYSTEM::SBAS;
        case (ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::QZSS>::identifier) :
            return SATELLITE_SYSTEM::QZSS;
        case (ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::BDS>::identifier) :
            return SATELLITE_SYSTEM::BDS;
        default : throw std::runtime_error
            ("ngpt::charToSatSys -> Invalid Satellite System Identifier!!");
    }
}
