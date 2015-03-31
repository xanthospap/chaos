#ifndef __SATELLITE_SYSTEMS__
#define __SATELLITE_SYSTEMS__

namespace ngpt {

/** @brief Enumeration type for Satellite Systems
 */
enum class SAT_SYS : uint16_t {
    GPS,
    GLONASS,
    GALILEO,
    SBAS,
    BDS,
    QZSS,
    MIXED,
    UNKNOWN
};

/** @brief Generalized template class for a satellite system
 */
template<SAT_SYS E>
class SatelliteSystem
{};

/** @brief Specialized template class for Gps satellite system
 */
template<>
class SatelliteSystem<SAT_SYS::GPS>
{
private:
    static constexpr char identifier { 'G' };
};

/** @brief Specialized template class for Glonass satellite system
 */
template<>
class SatelliteSystem<SAT_SYS::GLONASS>
{
private:
    static const char identifier { 'R' };
};

/** @brief Specialized template class for Galileo satellite system
 */
template<>
class SatelliteSystem<SAT_SYS::GALILEO>
{
private:
    static const char identifier { 'E' };
};

/** @brief Specialized template class for Sbas satellite system
 */
template<>
class SatelliteSystem<SAT_SYS::SBAS>
{
private:
    static const char identifier { 'S' };
};

/** @brief Specialized template class for Beidou satellite system
 */
template<>
class SatelliteSystem<SAT_SYS::BDS>
{
private:
    static const char identifier { 'C' };
};

/** @brief Specialized template class for QZSS satellite system
 */
template<>
class SatelliteSystem<SAT_SYS::QZSS>
{
private:
    static const char identifier { 'J' };
};

typedef SatelliteSystem<SAT_SYS::GPS>     Gps;
typedef SatelliteSystem<SAT_SYS::GLONASS> Glonass;
typedef SatelliteSystem<SAT_SYS::GALILEO> Galileo;
typedef SatelliteSystem<SAT_SYS::SBAS>    Sbas;
typedef SatelliteSystem<SAT_SYS::BDS>     Beidou;
typedef SatelliteSystem<SAT_SYS::QZSS>    Qzss;

} // end namespace gnpt

#endif
