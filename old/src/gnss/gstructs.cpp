#include <iostream>
#include "gstructs.hpp"
#include "varutils.hpp"

using std::string;
using namespace ngpt;

////////////////////////////////////////////////////////////////////////////////
//                                                             SATELLITE SYSTEMS
////////////////////////////////////////////////////////////////////////////////

/**
 * @details Resolve the satellite system of from a given char. In case of sucess, 
 *          status is set to zero. If the conversion fails,
 *          then SatelliteSystem::Unknown is returned. The translation
 *          follows the identifiers specified in RINEX v3 (@cite rnx302 ).
 *
 * @param[in]  c      the input char to be translated (\c char).
 * @param[out] status the status of the translation (\c int).
 * @return            if the status is zero, then the function returns the Satellite
 *                    System corresponging to the input character; else 
 *                    SatelliteSystem::Unknown is returned (\c SatelliteSystem).
 *
 * @note
 *  - In case the returned status (\c status) is not zero, then the Satellite System
 *    returned is SatelliteSystem::Unknown.
 *  - The status of the function (i.e. parameter \c status) can be:
 *    Returned Integer | Status
 *    -----------------|---------------------------------
 *                   0 | success; translation ok 
 *                   1 | string doesn't match any identifier
 *
 * @attention A whitespace (as input string) is interpreted as SatelliteSystem::Gps.
 *            This is for backwards compatibility with Rinex versions < 3.
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
SatelliteSystem ngpt::c2satsys (const char& c, int& status)
{

  /* function status */
  status = 0;

  /* match the string to a satellite system */
  switch (c) {
    case ('G') :
    case (' ') :  return SatelliteSystem::Gps;
    case ('R') :  return SatelliteSystem::Glonass;
    case ('E') :  return SatelliteSystem::Galileo;
    case ('C') :  return SatelliteSystem::BDS;
    case ('J') :  return SatelliteSystem::Qzss;
    case ('S') :  return SatelliteSystem::Sbas;
    case ('M') :  return SatelliteSystem::Mixed;
    case ('*') :  return SatelliteSystem::Any;
    case ('?') :  return SatelliteSystem::Unknown;
    default    :  status = 1;
  }

  return SatelliteSystem::Unknown;
}

/**
 * @details Resolve the satellite system of from a given string (actualy a 
 *          char). In case of sucess, status is set to zero. If the conversion fails,
 *          then SatelliteSystem::Unknown is returned. The translation
 *          follows the identifiers specified in RINEX v3 (@cite rnx302 ).
 *
 * @param[in]  str    the input string to be translated (\c std::string).
 * @param[out] status the status of the translation (\c int).
 * @return            if the status is zero, then the function returns the Satellite
 *                    System corresponging to the input character; else 
 *                    SatelliteSystem::Unknown is returned (\c SatelliteSystem).
 *
 * @note
 *  - In case the returned status (\c status) is not zero, then the Satellite System
 *    returned is SatelliteSystem::Unknown.
 *  - The status of the function (i.e. parameter \c status) can be:
 *    Returned Integer | Status
 *    -----------------|---------------------------------
 *                   0 | success; translation ok 
 *                   1 | string doesn't match any identifier
 *                   2 | string has size > 1
 *
 * @attention A whitespace (as input string) is interpreted as SatelliteSystem::Gps.
 *            This is for backwards compatibility with Rinex versions < 3.
 *
 * @note this is a wrapper for function ngpt::c2satsys to handle std::strings.
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
SatelliteSystem ngpt::str2satsys (const string& str, int& status)
{

  /* function status */
  status = 0;

  /* if the string has size > 1, then something is wrong */
  if (str.size()>1) {
    // std::cerr<<"Unknown Satellite System Identifier: ["<<str<<"]";
    status = 2;
    return SatelliteSystem::Unknown;
  }

  return ngpt::c2satsys (str[0], status);
}

/**
 * @details Translate a Satellite System to a char. The translation
 *          follows the identifiers specified in RINEX v3 (@cite rnx302 ).
 *
 * @param[in]  sat  the satellite system be translated (\c ngpt::SatelliteSystem).
 * @return          the function returns the identifier character of the input 
 *                  satellite system (\c char).
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
char ngpt::satsys2c (const SatelliteSystem& sat)
{
  /* match the string to a satellite system */
  switch (sat) {
    case (SatelliteSystem::Gps)     : return 'G';
    case (SatelliteSystem::Glonass) : return 'R';
    case (SatelliteSystem::Galileo) : return 'E';
    case (SatelliteSystem::BDS)     : return 'C';
    case (SatelliteSystem::Qzss)    : return 'J';
    case (SatelliteSystem::Sbas)    : return 'S';
    case (SatelliteSystem::Mixed)   : return 'M';
    case (SatelliteSystem::Any)     : return '*';
    case (SatelliteSystem::Unknown) :
    default                         : return '?';
  }
}

/**
 * @details Translate a Satellite System to a string (actualy a char). The translation
 *          follows the identifiers specified in RINEX v3 (@cite rnx302 ).
 *
 * @param[in]  sat  the satellite system be translated (\c ngpt::SatelliteSystem).
 * @return          the function returns the identifier character of the input 
 *                  satellite system (\c std::string). This has always a size of 
 *                  1 (character).
 *
 * @note This is a wrapper for function ngpt::satsys2c to handle std::strings.
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
string ngpt::satsys2str (const SatelliteSystem& sat)
{
  /* match the string to a satellite system */
  return std::string (1,ngpt::satsys2c (sat));
}

////////////////////////////////////////////////////////////////////////////////
//                                                                 CARRIER BANDS
////////////////////////////////////////////////////////////////////////////////

/**
 * @details Convert carrier band to a string. 
 *
 * @param[in]  cb   the input carrier band to be translated (\c CarrierBand).
 * @return          the function returns the string corresponding to the
 *                  given carrier band (\c SatelliteSystem).
 *
 */
std::string ngpt::carrierband2str(const CarrierBand& cb)
{

  /* match the carrier band to a string */
  switch (cb) {
    case (CarrierBand::Zero)    : return "Zero";
    case (CarrierBand::L1)      : return "L1";
    case (CarrierBand::L2)      : return "L2";
    case (CarrierBand::L5)      : return "L5";
    case (CarrierBand::G1)      : return "G1";
    case (CarrierBand::G2)      : return "G2";
    case (CarrierBand::G3)      : return "G3";
    case (CarrierBand::E5b)     : return "E5b";
    case (CarrierBand::E5)      : return "E5";
    case (CarrierBand::E6)      : return "E6";
    case (CarrierBand::B1)      : return "B1";
    case (CarrierBand::B2)      : return "B2";
    case (CarrierBand::B3)      : return "B3";
    case (CarrierBand::Any)     : return "*";
    case (CarrierBand::Unknown) :
    default                     : return "?";
  }

  /* all done */
}

/**
 * @details String to carrier band transformation. Trailing and leading spaces 
 *          are ignored. 
 *
 * Status:
 * ok = 0 -> sucess
 * ok = 1 -> failure; the returned string is Nan.
 *
 * @warning THIS FUNCTION IS INCOMPLETE.
 *
 */
CarrierBand ngpt::str2carrierband (const string& cstr, int& ok)
{

  /* function status */
  ok = 0;

  /* same string ignoring leading/trailing spaces */
  string str = stripstr (cstr);

  /* match the carrier band to a string */
  if (str == "?")
    return CarrierBand::Unknown;
  else if (str=="*")
    return CarrierBand::Any;
  else if (str=="Zero")
    return CarrierBand::Zero;
  else if (str=="L1")
    return CarrierBand::L1;
  else if (str=="L2")
    return CarrierBand::L2;
  else if (str=="L5")
    return CarrierBand::L5;
  else if (str=="G1")
    return CarrierBand::G1;
  else if (str=="G2")
    return CarrierBand::G2;
  else if (str=="E5b")
    return CarrierBand::E5b;
  else if (str=="E5")
    return CarrierBand::E5;
  else if (str=="E6")
    return CarrierBand::E6;
  else {
    ok = 1;
    return CarrierBand::Unknown;
  }

  /* all done */
}

/**
 * @details Match an integer to a valid CarrierBand enumerator. To perform the
 *          match, we need to know the satellite system, which must be provided
 *          at input. The integers corresponding to the band identifiers are
 *          taken from @cite rnx302 , <i>Table TABLE A2, GNSS OBSERVATION DATA FILE -
 *          HEADER SECTION DESCRIPTION</i>. This functio is usefule when reading
 *          carrier band information from input (e.g RINEX files). If the match
 *          is not possible, then CarrierBand::Unknown is returned and the status
 *          is set to 1 (else the status is set to 0).
 *
 * @param[in]  sat    the satellite system which emmited the observation at the 
 *                    carrier (\c SatelliteSystem).
 * @param[in]  band   the carrier band as integer (which is dubious) that needs
 *                    to be resolved (\c int).
 * @param[out] status the status of the function.
 * @return            a carrier band enumerator, corresponding to the combination
 *                    of \c band plus \c sat.
 *
 * @note The status of the function (i.e. parameter \c status) can be:
 *  Integer | Status
 * ---------|------------
 *       -1 | satellite system in ambiguous (i.e Any or Unknown); returned value : CarrierBand::Unknown
 *        0 | success
 *        1 | error; band not in range [0-8] (returned value: CarrierBand::Unknown)
 *        2 | error; satellite system and band do not match (returned value: CarrierBand::Unknown)
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
CarrierBand ngpt::int2carrierband (const SatelliteSystem& sat,const int& band,
                                  int& status)
{
  /* pre-set status */
  status = 1;

  /* obvious mistake */
  if (band > 8 || band < 0)
    return CarrierBand::Unknown;
  
  /* match the satellite system */
  switch (sat) {

    case (SatelliteSystem::Gps)     :
      switch (band) {
        case (1) : status=0; return CarrierBand::L1;
        case (2) : status=0; return CarrierBand::L2;
        case (5) : status=0; return CarrierBand::L5;
        default  : status=2; return CarrierBand::Unknown;
      }
    case (SatelliteSystem::Glonass) :
      switch (band) {
        case (1) : status=0; return CarrierBand::G1;
        case (2) : status=0; return CarrierBand::G2;
        case (3) : status=0; return CarrierBand::G3;
        default  : status=2; return CarrierBand::Unknown;
      }
    case (SatelliteSystem::Galileo) :
      switch (band) {
        case (1) : status=0; return CarrierBand::E1;
        case (5) : status=0; return CarrierBand::E5a;
        case (7) : status=0; return CarrierBand::E5b;
        case (8) : status=0; return CarrierBand::E5;
        case (6) : status=0; return CarrierBand::E6;
        default  : status=2; return CarrierBand::Unknown;
      }
    case (SatelliteSystem::BDS)  :
      switch (band) {
        case (1) : status=0; return CarrierBand::B1;
        case (7) : status=0; return CarrierBand::B2;
        case (6) : status=0; return CarrierBand::B3;
        default  : status=2; return CarrierBand::Unknown;
      }
    case (SatelliteSystem::Qzss)    :
      switch (band) {
        case (1) : status=0; return CarrierBand::L1;
        case (2) : status=0; return CarrierBand::L2;
        case (5) : status=0; return CarrierBand::L5;
        case (6) : status=0; return CarrierBand::LEX;
        default  : status=2; return CarrierBand::Unknown;
      }
    case (SatelliteSystem::Sbas)    :
      switch (band) {
        case (1) : status=0; return CarrierBand::L1;
        case (5) : status=0; return CarrierBand::L5;
        default  : status=2; return CarrierBand::Unknown;
      }
    case (SatelliteSystem::Any)     :/* return CarrierBand::Unknown;*/
    case (SatelliteSystem::Unknown) : return CarrierBand::Unknown;

    default : {
      status=3;
      return CarrierBand::Unknown;
    }
  }

}

////////////////////////////////////////////////////////////////////////////////
//                                                             OBSERVATION TYPES
////////////////////////////////////////////////////////////////////////////////

/**
 * @details Convert an observation type to a char. The observation types are
 *          matched to characters, exactly as described in @cite rnx302 .
 *          Two extra types and identifiers are introduced to match an unknown and
 *          any observation type.
 *
 * @param[in]  ot     the input observation type to be translated (\c ObservationType).
 * @return            the character corresponding to the input observation type (\c char).
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
char ngpt::obstype2c(const ObservationType& ot)
{

  /* match the observation type to a string */
  switch (ot) {
    case (ObservationType::Range)    : return 'C';
    case (ObservationType::Phase)    : return 'L';
    case (ObservationType::Doppler)  : return 'D';
    case (ObservationType::SNR)      : return 'S';
    case (ObservationType::Channel)  : return 'X';
    case (ObservationType::Iono)     : return 'I';
    case (ObservationType::Any)      : return '*';
    case (ObservationType::Unknown)  :
    default                          : return '?';
  }

  /* all done */
}

/**
 * @details Convert an observation type to a string. The observation types are
 *          matched to length-1 strings, exactly as described in @cite rnx302 .
 *          Two extra types and identifiers are introduced to match an unknown and
 *          any observation type.
 *
 * @param[in]  ot     the input observation type to be translated (\c ObservationType).
 * @return            the string corresponding to the input observation type (\c std::string).
 *
 * @note This is a wrapper for function ngpt::satsys2c to handle std::strings.
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
std::string ngpt::obstype2str(const ObservationType& ot)
{

  /* match the observation type to a string */
  return std::string (1,ngpt::obstype2c (ot));

  /* all done */
}

/**
 * @details Convert a char to valid observation type enumerator. The observation 
 *          types are matched to characters, exactly as described in @cite rnx302 .
 *          Two extra types and identifiers are introduced to match an unknown and
 *          any observation type. If status is other than 0, an error has occured.
 *
 * @param[in]  c      the input character to be translated to an observation type 
 *                    enumerator (\c char).
 * @param[out] status the status of the function (\c int).
 * @return            the observation type corresponding to the input character. In case
 *                    of error (i.e. ok!=0) ObservationType::Unknown is returned
 *                    (\c ObservationType).
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
ObservationType ngpt::c2obstype(const char& c, int& status)
{

  /* function status */
  status = 0;

  /* match the observation type to a string */
  switch (c) {
    case ('C') : return ObservationType::Range;
    case ('L') : return ObservationType::Phase;
    case ('D') : return ObservationType::Doppler;
    case ('S') : return ObservationType::SNR;
    case ('X') : return ObservationType::Channel;
    case ('I') : return ObservationType::Iono;
    case ('*') : return ObservationType::Any;
    case ('?') : return ObservationType::Unknown;
    default    : status = 1;
  }

  return ObservationType::Unknown;

  /* all done */
}

/**
 * @details Convert an std::string to valid observation type enumerator. The observation 
 *          types are matched to length-1 strings, exactly as described in @cite rnx302 .
 *          Two extra types and identifiers are introduced to match an unknown and
 *          any observation type. If status is other than 0, an error has occured.
 *          The string is trimmed of from any leading and/or trailing whitespaces.
 *
 * @param[in]  cstr   the input string to be rtanslated to an observation type 
 *                    enumerator (\c std::string).
 * @param[out] status the status of the function (\c int).
 * @return            the observation type corresponding to the input string. In case
 *                    of error (i.e. ok!=0) ObservationType::Unknown is returned
 *                    (\c ObservationType).
 *
 * @note This is a wrapper for function ngpt::c2obstype to handle std::strings.
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
ObservationType ngpt::str2obstype(const std::string& cstr, int& status)
{

  /* function status */
  status = 0;

  /* strip the string of from leading/trailing whitespaces */
  string str = stripstr (cstr);

  if (str.size ()!=1) {
    status = 1;
    return ObservationType::Unknown;
  }

  return ngpt::c2obstype (str[0],status);
}

////////////////////////////////////////////////////////////////////////////////
//                                                   TRACKING CODES / ATTRIBUTES
////////////////////////////////////////////////////////////////////////////////

/**
 * @details Convert a tracking code to a char. Note that the chars representing
 *          the tracking codes obey the rules defined in @cite rnx302 .
 *
 * @param[in] tc the tracking code to translate (\c TrackingCode).
 * @return       the tracking code as character (\c char).
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
char ngpt::trackingcode2c(const TrackingCode& tc)
{

  /* match the tracking code to a string */
  switch (tc) {
    case (TrackingCode::P)       : return 'P';
    case (TrackingCode::C)       : return 'C';
    case (TrackingCode::D)       : return 'D';
    case (TrackingCode::Y)       : return 'Y';
    case (TrackingCode::M)       : return 'M';
    case (TrackingCode::N)       : return 'N';
    case (TrackingCode::A)       : return 'A';
    case (TrackingCode::B)       : return 'B';
    case (TrackingCode::I)       : return 'I';
    case (TrackingCode::Q)       : return 'Q';
    case (TrackingCode::S)       : return 'S';
    case (TrackingCode::L)       : return 'L';
    case (TrackingCode::X)       : return 'X';
    case (TrackingCode::W)       : return 'W';
    case (TrackingCode::Z)       : return 'Z';
    case (TrackingCode::Any)     : return '*';
    case (TrackingCode::Unknown) :
    default                      : return '?';
  }

  /* all done */
}

/**
 * @details Convert a tracking code to a string. Note that the chars representing
 *          the tracking codes obey the rules defined in @cite rnx302 .
 *
 * @param[in] tc the tracking code to translate (\c TrackingCode).
 * @return       the tracking code as character (\c string).
 *
 * @note This is a wrapper for function ngpt::trackingcode2c to handle std::strings.
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
std::string ngpt::trackingcode2str(const TrackingCode& tc)
{

  /* match the tracking code to a string */
  return std::string (1,ngpt::trackingcode2c (tc));

  /* all done */
}

/**
 * @details Transform a char to a valid trackingcode enumerator. Note 
 *          that the chars representing the tracking codes obey the rules 
 *          defined in @cite rnx302 . In case the input char does not match any
 *          valid string, then status is set to 1 and TrackingCode::Unknown is
 *          returned.
 * 
 * @param[in]  c      the input character to be translated (\c char).
 * @param[out] status the status of the transformation (\c int).
 * @return            a tracking code enumerator corresponding to the input char
 *                    (\c TrackingCode).
 *
 * @warning In RINEX v3.xx, the attribute can be left blank if not known. So a
 *          whitespace char is mapped to TrackingCode::Unknown and the status is zero.
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
TrackingCode ngpt::c2trackingcode(const char& c, int& status)
{

  /* function status */
  status = 0;

  /* match the tracking code to a string */
  switch (c) {
    case ('P') : return TrackingCode::P;
    case ('C') : return TrackingCode::C;
    case ('D') : return TrackingCode::D;
    case ('Y') : return TrackingCode::Y;
    case ('M') : return TrackingCode::M;
    case ('N') : return TrackingCode::N;
    case ('A') : return TrackingCode::A;
    case ('B') : return TrackingCode::B;
    case ('I') : return TrackingCode::I;
    case ('Q') : return TrackingCode::Q;
    case ('S') : return TrackingCode::S;
    case ('L') : return TrackingCode::L;
    case ('X') : return TrackingCode::X;
    case ('W') : return TrackingCode::W;
    case ('Z') : return TrackingCode::Z;
    case ('*') : return TrackingCode::Any;
    case (' ') : 
    case ('?') : return TrackingCode::Unknown;
    default    : status = 1;
  }

  return TrackingCode::Unknown;
}

/**
 * @details Transform an std::string to a valid trackingcode enumerator. Note 
 *          that the chars representing the tracking codes obey the rules 
 *          defined in @cite rnx302 . In case the input string does not match any
 *          valid string, then status is set to 1 and TrackingCode::Unknown is
 *          returned. The input string is stripped of from any leading/trailing
 *          whitespaces.
 * 
 * @param[in] cstr    the input string to be translated (\c string).
 * @param[out] status the status of the transformation (\c int).
 * @return            a tracking code enumerator corresponding to the input string
 *                    (\c TrackingCode).
 *
 * @note This is a wrapper for function ngpt::c2trackingcode to handle std::strings.
 *
 * @warning In RINEX v3.xx, the attribute can be left blank if not known. So a
 *          whitespace char is mapped to TrackingCode::Unknown, and the status is zero.
 *
 * Reference @cite rnx302 , Table TABLE A2
 *
 */
TrackingCode ngpt::str2trackingcode(const string& cstr, int& status)
{

  /* function status */
  status = 0;

  /* strip the string of from leading/trailing whitespaces */
  string str = stripstr (cstr);

  if (!str.size ()) return TrackingCode::Unknown;

  /* match the tracking code to a string */
  return ngpt::c2trackingcode (str[0],status);
}

////////////////////////////////////////////////////////////////////////////////
//                                                        SATELLITE BLOCK MODELS
////////////////////////////////////////////////////////////////////////////////

/**
 * @details Convert a satellite block model to a valid string.
 * 
 * Status:
 * ok = 0 -> sucess
 * ok = 1 -> failure; string "Nan" is returned
 */
string ngpt::satblockmodel2str (const SatelliteBlockModel& sbm, int& ok)
{

  /* function status */
  ok = 0;

  /* match the satellite block model to a string */
  switch (sbm) {
    case (SatelliteBlockModel::BLOCK_I)    :return "BLOCK I";
    case (SatelliteBlockModel::BLOCK_II)   :return "BLOCK II";
    case (SatelliteBlockModel::BLOCK_IIA)  :return "BLOCK IIA";
    case (SatelliteBlockModel::BLOCK_IIR_A):return "BLOCK IIR-A";
    case (SatelliteBlockModel::BLOCK_IIR_B):return "BLOCK IIR-B";
    case (SatelliteBlockModel::BLOCK_IIR_M):return "BLOCK IIR-M";
    case (SatelliteBlockModel::BLOCK_IIF)  :return "BLOCK IIF";
    case (SatelliteBlockModel::BLOCK_IIIA) :return "BLOCK IIIA";
    case (SatelliteBlockModel::GALILEO_0A) :return "GALILEO-0A";
    case (SatelliteBlockModel::GALILEO_0B) :return "GALILEO-0B";
    case (SatelliteBlockModel::GALILEO_1)  :return "GALILEO-1";
    case (SatelliteBlockModel::GLONASS)    :return "GLONASS";
    case (SatelliteBlockModel::GLONASS_M)  :return "GLONASS-M";
    case (SatelliteBlockModel::GLONASS_K1) :return "GLONASS-K1";
    case (SatelliteBlockModel::GLONASS_K2) :return "GLONASS-K2";
    case (SatelliteBlockModel::QZSS)       :return "QZSS";
    case (SatelliteBlockModel::Unknown)    :return "Unknown";
    default: {
      //std::cerr<<"Unknown/Undefined Block Model";
      ok = 1;
    }
  }

  /* if we have reached this point, satellite block model is unresolved */
  return "Nan";
}

/*!
 * Convert a string to a valid satellite block model.
 * 
 * Status:
 * ok = 0 -> sucess
 * ok = 1 -> failure; the returned model is set to unknown
 */
SatelliteBlockModel ngpt::str2satblockmodel(const string& model,int& ok)
{

  /* function status */
  ok = 0;

  /* the string must match exaclty the model; strip off any leading
  or trailing whitespaces to compare better */
  string str = stripstr (model);

  /* match the string to a satellite block model */
  if (str=="BLOCK-I")
    return SatelliteBlockModel::BLOCK_I;
  else if (str=="BLOCK II")
    return SatelliteBlockModel::BLOCK_II;
  else if (str=="BLOCK IIA")
    return SatelliteBlockModel::BLOCK_IIA;
  else if (str=="BLOCK IIR-A")
    return SatelliteBlockModel::BLOCK_IIR_A;
  else if (str=="BLOCK IIR-B")
    return SatelliteBlockModel::BLOCK_IIR_B;
  else if (str=="BLOCK IIR-M")
    return SatelliteBlockModel::BLOCK_IIR_M;
  else if (str=="BLOCK IIF")
    return SatelliteBlockModel::BLOCK_IIF;
  else if (str=="BLOCK IIIA")
    return SatelliteBlockModel::BLOCK_IIIA;
  else if (str=="GALILEO-0A")
    return SatelliteBlockModel::GALILEO_0A;
  else if (str=="GALILEO-0B")
    return SatelliteBlockModel::GALILEO_0B;
  else if (str=="GALILEO-1")
    return SatelliteBlockModel::GALILEO_1;
  else if (str=="GLONASS")
    return SatelliteBlockModel::GLONASS;
  else if (str=="GLONASS M")
    return SatelliteBlockModel::GLONASS_M;
  else if (str=="GLONASS K1")
    return SatelliteBlockModel::GLONASS_K1;
  else if (str=="GLONASS K2")
    return SatelliteBlockModel::GLONASS_K2;
  else if (str=="QZSS")
    return SatelliteBlockModel::QZSS;
  else {
    //std::cerr<<"Unknown/Undefined Block Model"<<model;
    ok = 1;
    return SatelliteBlockModel::Unknown;
  }

  /* all done */
}

////////////////////////////////////////////////////////////////////////////////
//                                                        DATA DIFFERENCE LEVELS
////////////////////////////////////////////////////////////////////////////////
/*! 
 * Difference level to string
 */
std::string ngpt::diflevel2string (const DifferenceLevel& dl)
{
  switch (dl) {
    case (DifferenceLevel::Unknown) : return "Unknown";
    case (DifferenceLevel::Zero)    : return "Zero   ";
    case (DifferenceLevel::Single)  : return "Single ";
    case (DifferenceLevel::Double)  : return "Double ";
    case (DifferenceLevel::Triple)  : return "Triple ";
  }

  /* will never reach here! */
  return "Unknown";
}

/*!
 * Transform a string to a difference level enum
 * 
 * Status:
 * ok = 0 -> sucess
 * ok = 1 -> failure; the returned string is Nan.
 */
DifferenceLevel ngpt::string2diflevel (const string& cstr, int& ok)
{

  /* function status */
  ok = 0;

  /* strip the string of from leading/trailing whitespaces */
  string str = stripstr (cstr);

  if (str=="Unknown")
    return (DifferenceLevel::Unknown);
  else if (str == "Zero")
    return (DifferenceLevel::Zero);
  else if (str == "Single")
    return (DifferenceLevel::Single);
  else if (str == "Double")
    return (DifferenceLevel::Double);
  else if (str == "Triple")
    return (DifferenceLevel::Triple);
  else {
      ok = 1;
      return DifferenceLevel::Unknown;
  }

  /* all done */
}

/*! 
 * Given a string of type e.g. G01, resolve satellite system 
 * and carrier band; reference 
 * http://igscb.jpl.nasa.gov/igscb/station/general/antex14.txt
 *
 * If the function succedes in resolving both satellite system 
 * and the carrier band, ss and cb are set to the right values and
 * zero is returned.
 * If the function fails to match a combination, both satellite system
 * and carrier band are set to Unknown, and 1 is returned.
 *//*
int ngpt::resolvetype (const string& str,SatelliteSystem& ss,CarrierBand& cb)
{
  
  // function status 
  int status = 0;

  // match the string to any of the following combinations 
  if (str=="G01") {
    ss=SatelliteSystem::Gps;
    cb=CarrierBand::L1;
  } else if (str=="G02") {
    ss=SatelliteSystem::Gps;
    cb=CarrierBand::L2;
  } else if (str=="G05") {
    ss=SatelliteSystem::Gps;
    cb=CarrierBand::L5;
  } else if (str=="R01") {
    ss=SatelliteSystem::Glonass;
    cb=CarrierBand::L1;
  } else if (str=="R02") {
    ss=SatelliteSystem::Glonass;
    cb=CarrierBand::L2;
  } else if (str=="E01") {
    ss=SatelliteSystem::Galileo;
    cb=CarrierBand::L1;
  } else if (str=="E05") {
    ss=SatelliteSystem::Galileo;
    cb=CarrierBand::L5;
  } else if (str=="E07") {/home/xanthos/Desktop/phd_thesis/chaos-ngp/new/chaos_documented/src/gnss/gstructs.cpp:164
    ss=SatelliteSystem::Galileo;
    cb=CarrierBand::E5b;
  } else if (str=="E08") {
    ss=SatelliteSystem::Galileo;
    cb=CarrierBand::E5;
  } else if (str=="E06") {
    ss=SatelliteSystem::Galileo;
    cb=CarrierBand::E6;

  // cannot match !
  } else {
    // std::cerr<<"\n*** Cannot resolve Satellite System/Carier Band: "<<str;
    ss=SatelliteSystem::Unknown;
    cb=CarrierBand::Unknown;
    status = 1;
  }

  // return the status 
  return status;
}*/
