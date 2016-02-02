#include <iostream>
#include "gnssobs.hpp"

using namespace ngpt;

int main ()
{
    // make a _RawObs_
    _RawObs_ obs1 { SATELLITE_SYSTEM::GPS,
        OBSERVATION_TYPE::CARRIER_PHASE,
        1, 'C' };
        
    // print the _RawObs_
    std::cout << "\nObs = " << obs1.toString() << " with nominal frequency = "
    << obs1.nominalFrequency();
    
    // ok. let's make some observables
    GnssObs obs2 {obs1};
    GnssObs obs3 {{2.0,obs1}, {-2.0,obs1} };
    GnssObs obs4 {_RawObs_{SATELLITE_SYSTEM::GPS,OBSERVATION_TYPE::CARRIER_PHASE,1, 'C'}};
    GnssObs obs5 {
        {2.0,_RawObs_{SATELLITE_SYSTEM::GPS,OBSERVATION_TYPE::CARRIER_PHASE,1, 'C'}},
        {5.0,_RawObs_{SATELLITE_SYSTEM::GPS,OBSERVATION_TYPE::CARRIER_PHASE,1, 'C'}}
    };
    GnssObs obs6 {
        2.0,SATELLITE_SYSTEM::GPS,OBSERVATION_TYPE::CARRIER_PHASE,1, 'C',
        3.0,SATELLITE_SYSTEM::GPS,OBSERVATION_TYPE::CARRIER_PHASE,2, 'C',
        3.0,SATELLITE_SYSTEM::GPS,OBSERVATION_TYPE::CARRIER_PHASE,5, 'C'
    };
        
    // Nominal Frequencies and string conversions
    std::cout <<"\nNominal Frequency of Obs: " << obs2.toString() << " = " << obs2.nominalFrequency();
    std::cout <<"\nNominal Frequency of Obs: " << obs3.toString() << " = " << obs3.nominalFrequency();
    std::cout <<"\nNominal Frequency of Obs: " << obs4.toString() << " = " << obs4.nominalFrequency();
    std::cout <<"\nNominal Frequency of Obs: " << obs5.toString() << " = " << obs5.nominalFrequency();
    std::cout <<"\nNominal Frequency of Obs: " << obs6.toString() << " = " << obs6.nominalFrequency();
    
    std::cout<<"\n";
    return 0;
}