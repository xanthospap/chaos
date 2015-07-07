#ifndef _DTTMCON_
#define _DTTMCON_

/**
 * \file      receiver.hpp
 *
 * \version   0.10
 *
 * \author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * \date      MAY 2015
 *
 * \brief     Receiver Class for GNSS.
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

namespace datetime {

    /// Pi
    constexpr double DPI {3.141592653589793238462643e0};

    /// 2Pi
    constexpr double D2PI {6.283185307179586476925287e0};

    /// Radians to degrees 
    constexpr double DR2D {57.29577951308232087679815e0};

    /// Degrees to radians 
    constexpr double DD2R {1.745329251994329576923691e-2};

    /// Radians to arcseconds 
    constexpr double DR2AS {206264.8062470963551564734e0};

    /// Arcseconds to radians 
    constexpr double DAS2R {4.848136811095359935899141e-6};

    /// Seconds of time to radians 
    constexpr double DS2R {7.272205216643039903848712e-5};

    /// Arcseconds in a full circle 
    constexpr double TURNAS {1296000.0e0};

    /// Milliarcseconds to radians 
    constexpr double DMAS2R {DAS2R / 1e3};

    /// Length of tropical year B1900 (days) 
    constexpr double DTY {365.242198781e0};

    /// Seconds per day. 
    constexpr double DAYSEC {86400.0e0};

    /// Days per Julian year 
    constexpr double DJY {365.25e0};

    /// Days per Julian century 
    constexpr double DJC {36525.0e0};

    /// Days per Julian millennium 
    constexpr double DJM {365250.0e0};

    /// Reference epoch (J2000.0), Julian Date 
    constexpr double DJ00 {2451545.0e0};

    /// Julian Date of Modified Julian Date zero 
    constexpr double DJM0 {2400000.5e0};

    /// Reference epoch (J2000.0), Modified Julian Date 
    constexpr double DJM00 {51544.5e0};

    /// 1977 Jan 1.0 as MJD 
    constexpr double DJM77 {43144.0e0};

    /// TT minus TAI (s) 
    constexpr double TTMTAI {32.184e0};

    /// Astronomical unit (m) 
    constexpr double DAU {149597870e3};

    /// Speed of light (m/s) 
    constexpr double CMPS {299792458.0e0};

    /// Light time for 1 au (s) 
    constexpr double AULT {499.004782e0};

    /// Speed of light (AU per day) 
    constexpr double DC {DAYSEC / AULT};

    /// L_G = 1 - d(TT)/d(TCG) 
    constexpr double ELG {6.969290134e-10};

    /// L_B = 1 - d(TDB)/d(TCB), and TDB (s) at TAI 1977/1/1.0 
    constexpr double ELB {1.550519768e-8};
    constexpr double TDB0 {-6.55e-5};

    /// Schwarzschild radius of the Sun (au) 
    /// = 2 * 1.32712440041e20 / (2.99792458e8)^2 / 1.49597870700e11 
    constexpr double SRS {1.97412574336e-8};

} // end namespace datetime

#endif
