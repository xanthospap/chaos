#ifndef _GNSSOBS_HPP_
#define _GNSSOBS_HPP_

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

#include "satsys.hpp"
#include <vector>
#include <string>
// ONLY FOR DEBUGING //
#include <iostream>

namespace ngpt {
    
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

/// Given an observation type, return its identifier
char ObsTypeIdentifier(OBSERVATION_TYPE);

/// Struct to hold observable attributes. See RINEX v3.02
struct Attribute 
{char c_;};

/// Struct to hold a raw, rinex observable
struct _RawObs_ {
    SATELLITE_SYSTEM ss_; /// Satellite system
    OBSERVATION_TYPE ot_; /// Observation type
    short int        nf_; /// Nominal Frequency
    Attribute        at_; /// Attribute
    
    explicit _RawObs_ (SATELLITE_SYSTEM s,OBSERVATION_TYPE o,
            short int f,Attribute a) noexcept
        :ss_(s),ot_(o),nf_(f),at_(a)
    { };
    
    explicit _RawObs_ (SATELLITE_SYSTEM s,OBSERVATION_TYPE o,
            short int f,char c) noexcept
        :ss_(s),ot_(o),nf_(f),at_(Attribute{c})
    { };
    
    inline std::string toString() const
    {
        return std::string {'[',
            SatSysIdentifier(ss_),
            ObsTypeIdentifier(ot_),
            std::to_string(nf_)[0],
            at_.c_,
            ']'};
    }
    
    double nominalFrequency() const;
};

class GnssObs {
    typedef std::pair<double,_RawObs_> cobs_pair;
    
private:
    std::vector<std::pair<double,_RawObs_>> obs_;

public:
    /// Constructor given a list of _RawObs_
    /// e,g GnssObs obs { _RawObs_{...} };
    GnssObs(std::initializer_list<_RawObs_> ro)
    { for (auto o : ro) obs_.emplace_back(cobs_pair{1.0e0,o}); }
    
    /// Constructor given a list of [coefficient,_RawObs_] pairs.
    /// e.g. GnssObs obs { {1.5,_RawObs_{...}} };
    GnssObs(std::initializer_list<cobs_pair> ro)
    { for (auto o : ro) obs_.emplace_back(o); }
    
    /// Ending condition for variadic constructor.
    GnssObs(double c,SATELLITE_SYSTEM s,OBSERVATION_TYPE o,short int f,char a)
        noexcept : obs_()
    {
        std::cout<< "\n__PRETTY_FUNCTION__" << "Final call to constructor ->" << _RawObs_(s,o,f,c).toString();
        obs_.emplace_back( cobs_pair{ c,_RawObs_{s,o,f,a} } ); 
    }
    /// Variadic Template constructor (recursive). 
    template<typename... Args>
    GnssObs(double c,SATELLITE_SYSTEM s,OBSERVATION_TYPE o,short int f,char a,
            Args... args)
    {
        std::cout<<"\n__PRETTY_FUNCTION__" << "GnssObs[Non-Recr] -> " << _RawObs_(s,o,f,c).toString();
        obs_.emplace_back( cobs_pair{ c,_RawObs_{s,o,f,a} } );
        addRawObs/*GnssObs*/(args...);
    }
    
    /// Return the number of raw observables.
    inline std::size_t numOfRawObservables() const noexcept
    { return obs_.size(); }
    
    /// Nominal frequency (simple linear combination).
    double nominalFrequency() const
    {
        double freq { .0e0 };
        for (auto& i : obs_)
            freq += i.first * i.second.nominalFrequency();
        return freq;
    }
    
    /// Convert to std::string.
    std::string toString() const
    {
        std::string str;
        for (auto& i : obs_)
            str += i.second.toString();
        return str;
    }
 
    /*   
    void addRawObs(double c,_RawObs_ ro)
    { obs_.emplace_back( cobs_pair{ c,ro } ); }
    template<typename... Args>
    void addRawObs(double c,_RawObs_ ro,Args... args)
    {
        obs_.emplace_back( cobs_pair{ c,ro } );
        addRawObs(args...);
    }
    */
    
    void addRawObs(double c,SATELLITE_SYSTEM s,OBSERVATION_TYPE o,short int f,
            char a)
    { 
        std::cout<<"\n__PRETTY_FUNCTION__" <<" addRawObs[Non-Recr] -> " << _RawObs_(s,o,f,c).toString();
        obs_.emplace_back( cobs_pair{ c,_RawObs_{s,o,f,a} } ); }
    template<typename... Args>
    void addRawObs(double c,SATELLITE_SYSTEM s,OBSERVATION_TYPE o,short int f,
            char a,Args... args)
    {
        std::cout<<"\n__PRETTY_FUNCTION__" <<" addRawObs[Variadic] -> " << _RawObs_(s,o,f,c).toString();
        obs_.emplace_back( cobs_pair{ c,_RawObs_{s,o,f,a} } );
        addRawObs(args...);
    }
    
};

}
#endif
