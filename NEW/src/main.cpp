#include <iostream>
#include "satsys.hpp"
#include "obstypes.hpp"
#include "attrib.hpp"
#include "freqs.hpp"
#include "obscodes.hpp"

using namespace ngpt;

int main ()
{
    SatelliteSystem<SAT_SYS::GALILEO> s1;
    //s1.foo();
    
    SatelliteSystem<SAT_SYS::GPS> s2;
    //s2.foo();
    
    Qzss s3;
    //s3.foo();
    
    SatelliteSystem<SAT_SYS::UNKNOWN> s4;
    //s4.foo();
    
    SatelliteSystem<SAT_SYS::GPS> s5 (SatelliteSystem<SAT_SYS::GPS>);
    
    
    constexpr ngpt::SAT_SYS SatSys = ngpt::SAT_SYS::GPS;
    ngpt::SatelliteSystem<SatSys> koko;
    
    std::cout << "Frequency for band1 of GPS is " << freq<SAT_SYS::GPS,1>() <<  "\n";
    
    //constexpr Attribute a {};
    //Attribute *a_ptr = &a;
    
    RawObs<SAT_SYS::GPS, 1> obs;
    
    std::cout <<"\n";
    return 0;
}
