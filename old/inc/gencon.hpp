#ifndef _GENCON_HPP
#define _GENCON_HPP

/**
 * @file      gencon.hpp
 *
 * @version   1.0-1b
 *
 * @author    xanthos@mail.ntua.gr <br>
 *            danast@mail.ntua.gr
 *
 * @date      December, 2014
 *
 * @brief     Definition of general use constants.
 *
 * @todo      
 *  -# Provide source / references
 *
 * @copyright No COPYRIGHT whatsoever.
 *
 * <b><center><hr>
 * National Technical University of Athens <br>
 *      Dionysos Satellite Observatory     <br>
 *        Higher Geodesy Laboratory        <br>
 *      http://dionysos.survey.ntua.gr
 * </center></b>
 */

/*! Pi */
/*#define*/constexpr double DPI        (3.141592653589793238462643);

/*! 2Pi */
/*#define*/constexpr double D2PI       (6.283185307179586476925287);

/*! Degrees to radians */
/*#define*/constexpr double DD2R       (1.745329251994329576923691e-2);

/*! Radians to seconds */
/*#define*/constexpr double DRAD2SEC   (86400e0 / D2PI);

/*! Radians to arcseconds */
/*#define*/constexpr double DR2AS      (206264.8062470963551564734);

/*! Arcseconds to radians */
/*#define*/constexpr double DAS2R      (4.848136811095359935899141e-6);

/*! Seconds of time to radians */
/*#define*/constexpr double DS2R       (7.272205216643039903848712e-5);

/*! Arcseconds in a full circle */
/*#define*/constexpr double TURNAS     (1296000.0e0);

/*! Milliarcseconds to radians */
/*#define*/constexpr double DMAS2R     (DAS2R / 1e3);

/*! Length of tropical year B1900 (days) */
/*#define*/constexpr double DTY        (365.242198781e0);

/*! Seconds per day. */
/*#define*/constexpr double DAYSEC     (86400.0e00);

/*! Days per Julian year */
/*#define*/constexpr double DJY        (365.25e0);

/*! Days per Julian century */
/*#define*/constexpr double DJC        (36525.0e0);

/*! Days per Julian millennium */
/*#define*/constexpr double DJM        (365250.0e0);

/*! Reference epoch (J2000.0), Julian Date */
/*#define*/constexpr double DJ00       (2451545.0e0);

/*! Julian Date of Modified Julian Date zero */
/*#define*/constexpr double DJM0       (2400000.5e0);

/*! Reference epoch (J2000.0), Modified Julian Date */
/*#define*/constexpr double DJM00      (51544.5e0);

/*! 1977 Jan 1.0 as MJD */
/*#define*/constexpr double DJM77      (43144.0e0);

/*! TT minus TAI (s) */
/*#define*/constexpr double TTMTAI     (32.184e0);

/*! AU (m) */
/*#define*/constexpr double DAU        (149597870e3);

/*! Speed of light (AU per day) */
/*#define*/constexpr double DC         (DAYSEC / 499.004782);

/*! L_G = 1 - d(TT)/d(TCG) */
/*#define*/constexpr double ELG        (6.969290134e-10);

/*! L_B = 1 - d(TDB)/d(TCB), and TDB (s) at TAI 1977/1/1.0 */
/*#define*/constexpr double ELB        (1.550519768e-8);

/*#define*/constexpr double TDB0       (-6.55e-5);

#endif
