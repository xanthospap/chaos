#ifndef _GENCON_HPP
#define _GENCON_HPP

/* ========================================================
** Ntua-Gnss-Processing Tools
** --------------------------------------------------------
** National Technical University of Athens
** Dionysos Satellite Observatory
** Higher Geodesy Laboratory
** --------------------------------------------------------
** File: gencon.hpp
** Created at: 09 December 2013
** Last Update:
*  =======================================================*/

/* Pi */
#define DPI (3.141592653589793238462643e0)

/* 2Pi */
#define D2PI (6.283185307179586476925287e0)

/* Degrees to radians */
#define DD2R (1.745329251994329576923691e-2)

/* Radians to seconds */
#define DRAD2SEC (86400e0 / D2PI)

/* Radians to arcseconds */
#define DR2AS (206264.8062470963551564734e0)

/* Arcseconds to radians */
#define DAS2R (4.848136811095359935899141e-6)

/* Seconds of time to radians */
#define DS2R (7.272205216643039903848712e-5)

/* Arcseconds in a full circle */
#define TURNAS (1296000.0e0)

/* Milliarcseconds to radians */
#define DMAS2R (DAS2R / 1e3)

/* Length of tropical year B1900 (days) */
#define DTY (365.242198781e0)

/* Seconds per day. */
#define DAYSEC (86400.0e00)

/* Days per Julian year */
#define DJY (365.25e0)

/* Days per Julian century */
#define DJC (36525.0e0)

/* Days per Julian millennium */
#define DJM (365250.0e0)

/* Reference epoch (J2000.0), Julian Date */
#define DJ00 (2451545.0e0)

/* Julian Date of Modified Julian Date zero */
#define DJM0 (2400000.5e0)

/* Reference epoch (J2000.0), Modified Julian Date */
#define DJM00 (51544.5e0)

/* 1977 Jan 1.0 as MJD */
#define DJM77 (43144.0e0)

/* TT minus TAI (s) */
#define TTMTAI (32.184e0)

/* AU (m) */
#define DAU (149597870e3)

/* Speed of light (AU per day) */
#define DC (DAYSEC / 499.004782e0)

/* L_G = 1 - d(TT)/d(TCG) */
#define ELG (6.969290134e-10)

/* L_B = 1 - d(TDB)/d(TCB), and TDB (s) at TAI 1977/1/1.0 */
#define ELB (1.550519768e-8)
#define TDB0 (-6.55e-5)

#endif
