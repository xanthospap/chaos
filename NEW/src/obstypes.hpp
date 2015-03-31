#ifndef __OBSERVATION_TYPES__
#define __OBSERVATION_TYPES__

namespace ngpt {

/** @brief Enumeration type for observation types.
 */
enum class OBSERVATION_TYPE : uint16_t {
    PSEUDORANGE,
    CARRIER_PHASE,
    DOPPLER,
    SIGNAL_STRENGTH,
    CHANNEL,
    IONO,
    UNKNOWN
};

/** @brief Generalized template class for an observation type
 */
template<OBSERVATION_TYPE E>
class ObservationType {};

/** @brief Specialized template for Pseudorange observation type
 */
template<>
class ObservationType<OBSERVATION_TYPE::PSEUDORANGE>
{
private:
    static constexpr char identifier { 'P' };
};

/** @brief Specialized template for Carrier Phase observation type
 */
template<>
class ObservationType<OBSERVATION_TYPE::CARRIER_PHASE>
{
private:
    static constexpr char identifier { 'L' };
};

/** @brief Specialized template for Doppler observation type
 */
template<>
class ObservationType<OBSERVATION_TYPE::DOPPLER>
{
private:
    static constexpr char identifier { 'D' };
};

/** @brief Specialized template for Signal Strength observation type
 */
template<>
class ObservationType<OBSERVATION_TYPE::SIGNAL_STRENGTH>
{
private:
    static constexpr char identifier { 'S' };
};

/** @brief Specialized template for Channel observation type; see
 *         The Receiver Independent Exchange Format Version 3.02, 
 *         5.13 Channel numbers as pseudo-observables
 */
template<>
class ObservationType<OBSERVATION_TYPE::CHANNEL>
{
private:
    static constexpr char identifier { 'X' };
};

/** @brief Specialized template for Iono observation type
 *         The Receiver Independent Exchange Format Version 3.02, 
 *         5.12 Ionosphere delay as pseudo-observables
 */
template<>
class ObservationType<OBSERVATION_TYPE::IONO>
{
private:
    static constexpr char identifier { 'I' };
};

typedef ObservationType<OBSERVATION_TYPE::PSEUDORANGE>     PseudoRange;
typedef ObservationType<OBSERVATION_TYPE::CARRIER_PHASE>   CarrierPhase;
typedef ObservationType<OBSERVATION_TYPE::DOPPLER>         Doppler;
typedef ObservationType<OBSERVATION_TYPE::SIGNAL_STRENGTH> SignalStrength;
typedef ObservationType<OBSERVATION_TYPE::CHANNEL>         Channel;
typedef ObservationType<OBSERVATION_TYPE::IONO>            Iono;

} // end namespace ngpt

#endif
