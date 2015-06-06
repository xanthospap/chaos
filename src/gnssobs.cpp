#include "gnssobs.hpp"
#include <stdexcept>

/** \details  Given an observation type enumerator, this function will return
 *            it's identifier (e.g. given ObservationType = Pseudeorange,
 *            the function will return 'C'). The identifier are taken from 
 *            RINEX v3.02.
 * 
 *  \param[in] o Input Observation type
 *  \return      A char, representing the observation type
 * 
 *  \throw       std::runtime_error if no matching observation type is found.
 */
char ngpt::ObsTypeIdentifier(ngpt::OBSERVATION_TYPE o)
{
    using ngpt::OBSERVATION_TYPE;
    
    switch ( o ) {
        case OBSERVATION_TYPE::PSEUDORANGE :
            return 'C';
        case OBSERVATION_TYPE::CARRIER_PHASE :
            return 'L';
        case OBSERVATION_TYPE::DOPPLER :
            return 'D';
        case OBSERVATION_TYPE::SIGNAL_STRENGTH :
            return 'S';
        case OBSERVATION_TYPE::IONOSPHERE_PHASE_DELAY :
            return 'I';
        case OBSERVATION_TYPE::RECEIVER_CHANNEL_NUMBER :
            return 'X';
        default:
            throw std::runtime_error
            ("ngpt::ObsTypeIdentifier -> Invalid Satellite System !!");
    }
}

double ngpt::_RawObs_::nominalFrequency() const
{
    using ngpt::SATELLITE_SYSTEM;

    switch (ss_) {
        case (SATELLITE_SYSTEM::GPS) : 
            return SatelliteSystemTraits<SATELLITE_SYSTEM::GPS>::nominal_frequency(nf_);
        case (SATELLITE_SYSTEM::GLONASS) :
            return SatelliteSystemTraits<SATELLITE_SYSTEM::GLONASS>::nominal_frequency(nf_);
        case SATELLITE_SYSTEM::SBAS :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::SBAS>::nominal_frequency(nf_);
        case SATELLITE_SYSTEM::GALILEO :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::GALILEO>::nominal_frequency(nf_);
        case SATELLITE_SYSTEM::BDS :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::BDS>::nominal_frequency(nf_);
        case SATELLITE_SYSTEM::QZSS :
            return ngpt::SatelliteSystemTraits<SATELLITE_SYSTEM::QZSS>::nominal_frequency(nf_);
        default:
            throw std::runtime_error
            ("ngpt::_RawObs_::nominalFrequency -> Invalid Satellite System !!");
    }
}
