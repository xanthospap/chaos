#ifndef __OBSERVATION_CODES__
#define __OBSERVATION_CODES__

#include "satsys.hpp"
#include "obstypes.hpp"
#include "attrib.hpp"

namespace ngpt {

template<
    ngpt::SAT_SYS SatSys,
    short int Freq
    //ngpt::Attribute *Atr
    >
class RawObs {
    public:
        RawObs():
            m_sat_sys {},
            m_frequency {Freq}
        {};
    private:
        ngpt::SatelliteSystem<SatSys> m_sat_sys;
        short int                     m_frequency;
        ngpt::Attribute               m_attribute;
};

}  // end namespace gnpt

#endif
