#include "gnssobs.hpp"
#include <stdexcept>
#include <cctype>

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

/** \details   The identifier are taken from 
 *            RINEX v3.02.
 * 
 *  \param[in] c Input char (representing an observation type)
 *  \return      Corresponding observation type.
 * 
 *  \throw       std::runtime_error if no matching observation type is found.
 */
ngpt::OBSERVATION_TYPE ngpt::charToObsType(char c)
{
    using ngpt::OBSERVATION_TYPE;
    
    switch ( c ) {
        case ('C') : return OBSERVATION_TYPE::PSEUDORANGE;
        case ('L') : return OBSERVATION_TYPE::CARRIER_PHASE;
        case ('D') : return OBSERVATION_TYPE::DOPPLER;
        case ('S') : return OBSERVATION_TYPE::SIGNAL_STRENGTH;
        case ('I') : return OBSERVATION_TYPE::IONOSPHERE_PHASE_DELAY;
        case ('X') : return OBSERVATION_TYPE::RECEIVER_CHANNEL_NUMBER;
        default    : throw std::runtime_error
            ("ngpt::charToObsType -> Invalid ObsType identifier !!");
    }
}

// TODO satllite system ??
std::vector<ngpt::GnssObs> ngpt::resolveRinexObservable(const char* str,
        const short int sz,std::vector<ngpt::SATELLITE_SYSTEM>& satss)
{
    if (sz<2 || sz>3)
        throw std::runtime_error
        ("ngpt::resolveRinexObservable -> Size too small!!");
    
    // First char is the observation type.
    ngpt::OBSERVATION_TYPE ot = ngpt::charToObsType(*str);
    
    // Second should be the (nominal) frequency
    // Perfectly safe in C++. In both the source and execution basic character
    // sets, the value of each character after 0 in the above list of decimal 
    // digits shall be one greater than the value of the previous.
    // See http://stackoverflow.com/questions/9416926/are-the-character-digits-0-9-required-to-have-contiguous-numeric-values
    if ( !isdigit(*(str+1)) )
        throw std::runtime_error
        ("ngpt::resolveRinexObservable -> Invalid frequency band !!");
    short int fr = *(str+1) - '0';
    
    // Attribute
    char at = (sz == 3) ? *(str+2) : '?';
    
    // Compile the Observables vector
    std::vector<ngpt::GnssObs> obsvec;
    for (auto ss : satss) {
        obsvec.emplace_back(1.0e0,ss,ot,fr,at);
    }

    // return the vector of observables
    return obsvec;
}
ngpt::GnssObs ngpt::resolveRinexObservable(const char* str,
        const short int sz,ngpt::SATELLITE_SYSTEM ss)
{
    if (sz<2 || sz>3)
        throw std::runtime_error
        ("ngpt::resolveRinexObservable -> Size too small!!");
    
    // First char is the observation type.
    ngpt::OBSERVATION_TYPE ot = ngpt::charToObsType(*str);
    
    // Second should be the (nominal) frequency
    // Perfectly safe in C++. In both the source and execution basic character
    // sets, the value of each character after 0 in the above list of decimal 
    // digits shall be one greater than the value of the previous.
    // See http://stackoverflow.com/questions/9416926/are-the-character-digits-0-9-required-to-have-contiguous-numeric-values
    if ( !isdigit(*(str+1)) )
        throw std::runtime_error
        ("ngpt::resolveRinexObservable -> Invalid frequency band !!");
    short int fr = *(str+1) - '0';
    
    // Attribute
    char at = (sz == 3) ? *(str+2) : '?';
    
    // Compile and return the Observables instance
    return ngpt::GnssObs { ngpt::_RawObs_{ss,ot,fr,at} };
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
