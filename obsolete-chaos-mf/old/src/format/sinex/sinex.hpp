#ifndef _NGPT_SINEX_HPP
#define _NGPT_SINEX_HPP

namespace ngpt {

class Sinex {
  public:

    enum class PARAMETER_TYPE : int {
      STAX,    ///< station X coordinate, m
      STAY,    ///< station Y coordinate, m
      STAZ,    ///< station Z coordinate, m
      VELX,    ///< station X velocity, m/y
      VELY,    ///< station Y velocity, m/y
      VELZ,    ///< station Z velocity, m/y
      XGC,     ///< geocenter X coordinate, m
      YGC,     ///< geocenter Y coordinate, m
      ZGC,     ///< geocenter Z coordinate, m
      RS_RA,   ///< radio source right ascension, rad
      RS_DE,   ///< radio source declination, rad
      RS_RAR,  ///< radio source right ascension, rad/y
      RS_DER,  ///< radio source declination rate, rad/y
      RS_PL,   ///< radio source parallax, rad
      LOD,     ///< length of day, ms
      UT,      ///< delta time UT1-UTC, ms
      XPO,     ///< X polar motion, mas
      YPO,     ///< Y polar motion, mas
      XPOR,    ///< X polar motion rate, mas/d
      YPOR,    ///< Y polar motion rate, mas/d
      NUT_LN,  ///< nutation correction in longitude, mas
      NUT_OB,  ///< nutation correction in obliquity, mas
      NUTRLN,  ///< nutation rate in longitude, mas/d
      NUTROB,  ///< nutation rate in obliquity, mas/d
//      NUT_X,   ///< nutation correction X, mas
//      NUT_Y,   ///< nutation correction Y, mas
//      NUTR_X,  ///< nutation rate in X mas/d
//      NUTR_Y,  ///< nutation rate in Y mas/d
      SAT__X,  ///< Satellite X coord., m
      SAT__Y,  ///< Satellite Y coord., m
      SAT__Z,  ///< Satellite Z coord., m
      SAT_VX,  ///< Satellite X velocity, m/s
      SAT_VY,  ///< Satellite Y velocity, m/s
      SAT_VZ,  ///< Satellite Z velocity, m/s
      SAT_RP,  ///< Radiation pressure
      SAT_GX,  ///< GX scale,
      SAT_GZ,  ///< GZ scale,
      SATYBI,  ///< GY bias, m/s2
      TROTOT,  ///< wet + dry Trop. delay, m
      TRODRY,  ///< dry Trop. delay, m
      TROWET,  ///< wet Trop. delay, m
      TGNTOT,  ///< troposphere gradient in north (wet + dry), m
      TGNWET,  ///< troposphere gradient in north (only wet), m
      TGNDRY,  ///< troposphere gradient in north (only dry), m
      TGETOT,  ///< troposphere gradient in east (wet + dry), m
      TGEWET,  ///< troposphere gradient in east (only wet), m
      TGEDRY,  ///< troposphere gradient in east (only dry), m
      RBIAS,   ///< range bias, m
      TBIAS,   ///< time bias, ms
      SBIAS,   ///< scale bias, ppb
      ZBIAS,   ///< troposphere bias at zenith, m
      AXI_OF,  ///< VLBI antenna axis offset, m
      SATA_Z,  ///< sat. antenna Z offset, m
      SATA_X,  ///< sat. antenna X offset, m
      SATA_Y,  ///< sat. antenna Y offset, m
//      CN,      ///< spherical harmonic coefficient C_nm 
//      SN,      ///< spherical harmonic coefficient S_nm 
      NUT_X,   ///< Nutation angle DELTA_X (correction to a priori model; new paradigm)
      NUT_Y,   ///< Nutation angle DELTA_Y (correction to a priori model; new paradigm)
      NUTR_X,  ///< Linear drift for nutation angle DELTA_X
      NUTR_Y,  ///< Linear drift for nutation angle DELTA Y
      CN,      ///< Normalized spherical harmonic coefficient of the Earth’s gravity field (cosine term)
      SN,      ///< Normalized spherical harmonic coefficient of the Earth’s gravity field (sine term)
    };

    enum class CONSTRAINT_CODE : char {
      FTC, ///< fixed/tight constraints (0)
      SC,  ///< significant constraints (1)
      UC,  ///< unconstrained (2)
    };

  private:
};

};

#endif
