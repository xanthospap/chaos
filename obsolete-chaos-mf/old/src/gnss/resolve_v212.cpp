#include <algorithm> 
#include "gobst.hpp"

using namespace ngpt;
using std::string;

/**
 * @details Resolve an observable type, from a string formated as in RinexV2.12 
 *          (see Table 1). Note that some observable types may be dubious. That is 
 *          because:<br>
 *          1) the tracking code is never clear (in most cases set to Unknown)<br>
 *          2) the satellite system is not clear (i.e. L1 may mean GPS L1 and/or
 *          GLONASS L1 etc).<br>
 *          For that reason, the function will actually return a vector
 *          containing all possible combinations that can be formed from
 *          the string. (i.e. if the string is "L1", then the returned
 *          vector will contain: GPS::L1,GLONASS::L1,GALILEO::E2-L1-E1(L1),SBAS::L1 ...
 *
 * @param[in]  cstr  The string to be resolved, holding a RINEX v2.x observable 
 *                   identifier (e.g. 'L1') (\c string).
 * @param[in]  ss    (optional) If the user wants to limit the number of possible
 *                   observables that can be formed by the input string, then a
 *                   pointer to a vector can be given, containing the satellite
 *                   systems of interest.
 * @return           A vector containing all posibble observables the input string
 *                   can represent.
 *
@verbatim
  +---------------------------------------------------------------------------+
  | System  Freq.Band  Frequency           RINEX 2-character Code             |
  |                              Ps.Range  Carr.Phase  Doppler  Sign.Strength |
  +---------------------------------------------------------------------------+
  | GPS     L1         1575.42     C1,P1       L1         D1         S1       |
  |         L2         1227.60     C2,P2       L2         D2         S2       |
  |         L5         1176.45      C5         L5         D5         S5       |
  |                                                                           |
  | Glonass G1         1602+k*9/16 C1,P1       L1         D1         S1       |
  |         G2         1246+k*7/16 C2,P2       L2         D2         S2       |
  |                                                                           |
  | Galileo E2-L1-E1   1575.42      C1         L1         D1         S1       |
  |         E5a        1176.45      C5         L5         D5         S5       |
  |         E5b        1207.140     C7         L7         D7         S7       |
  |         E5a+b      1191.795     C8         L8         D8         S8       |
  |         E6         1278.75      C6         L6         D6         S6       |
  |                                                                           |
  | SBAS    L1         1575.42      C1         L1         D1         S1       |
  |         L5         1176.45      C5         L5         D5         S5       |
  +---------------------------------------------------------------------------+
                                                                      [Table 1]

  11.3 New Observation Codes for GPS L1C,L2C,L1(C/A), Glonass G1(SA),G2(SA),
     and Compass

  +---------------------------------------------------------------------------+
  | System  Freq Code  Frequency             RINEX 2-Character Codes          |
  |             Channel           Ps.Range Carr.Phase  Doppler  Sign.Strength |
  +---------------------------------------------------------------------------+
  | GPS     L1   P,Y   1575.42       P1         L1         D1         S1      |
  |         L1   C/A   1575.42       CA         LA         DA         SA      |
  |         L1   L1C   1575.42       CB         LB         DB         SB      |
  |         L2   P,Y   1227.60      C2,P2       L2         D2         S2      |
  |         L2   L2C   1227.60       CC         LC         DC         SC      |
  |                                                                           |
  | Glonass G1   HA    1602+k*9/16   P1         L1         D1         S1      |
  |         G1   SA    1602+k*9/16   CA         LA         DA         SA      |
  |         G2   HA    1246+k*7/16   P2         L2         D2         S2      |
  |         G2   SA    1246+k*7/16   CD         LD         DD         SD      |
  |                                                                           |
  | Compass E2   I/Q                 C2         L2         D2         S2      |
  |         E5b  I/Q                 C7         L7         D7         S7      |
  |         E6   I/Q                 C6         L6         D6         S6      |
  +---------------------------------------------------------------------------+

  GPS     C2: CA+(P2-P1)  Pseudorange based on C/A(L1) and difference
                          P2-P1 or Y2-Y1

  Glonass SA: Based on standard accuracy code
          HA: Based on high accuracy code

@endverbatim
 *
 * @note 
 *    -# The following table shows how Rinex V2.12 strings describing range observations 
 *       are matched to a Rinex V3.xx strings.
 * <table>
 *  <tr>
 *    <th>Rinex V2.xx String</th>
 *    <th>Rinex V3.xx String</th>
 *  </tr>
 *  <tr>
 *    <td>C1</td>
 *    <td>G-C1C, R-C1C, E-C1?, S-C1C</td>
 *  </tr>
 *  <tr>
 *    <td>C1</td>
 *    <td>G-C1C, R-C1C, E-C1?, S-C1C</td>
 *  </tr>
 *  <tr>
 *    <td>C2</td>
 *    <td>G-C2C, R-C2C</td>
 *  </tr>
 *  <tr>
 *    <td>C5</td>
 *    <td>G-C5C, E-C5?, S-C5C</td>
 *  </tr>
 *  <tr>
 *    <td>C6</td>
 *    <td>E-C6?</td>
 *  </tr>
 *  <tr>
 *    <td>C7</td>
 *    <td>E-C7?</td>
 *  </tr>
 *  <tr>
 *    <td>C8</td>
 *    <td>E-C8?</td>
 *  </tr>
 *  <tr>
 *    <td>P1</td>
 *    <td>G-C1P, R-C1P</td>
 *  </tr>
 *  <tr>
 *    <td>P2</td>
 *    <td>G-C2P, R-C2P</td>
 *  </tr>
 * </table>
 * For phase, doppler and snr observables, the tracking codes / attributes are set to Unknown
 * -# This is an addition to the raw_obs::resolve_v2xx function, using the additions of RINEX v2.12
 *    based on @cite rnx212 . For a more detailed description see References.
 * -# In RINEX v2.12 (contrary to v2.11 and previous) satellite system COMPASS is defined. That
 *    means that COMPASS observables are possible and (if fit) will be returned by this function.
 *    Note that Compass Satellites/Observables use the identifier character 'C'.
 *
 * Reference @cite rnx212
 *
 */
std::vector<raw_obs> raw_obs::resolve_v212 (const std::string& cstr,std::vector<SatelliteSystem>* ss)
{
  // remove any trailing or leading whitespaces
  string str = stripstr (cstr);

  // pre-set status
  int /*status=0,*/ j;

  // initialize the vector
  std::vector<raw_obs> rtn;

  // the size of the string must be equal to 2; else exit
  if (str.size()!=2) return rtn;

  // cannot resolve tracking code either way
  auto utc = TrackingCode::Unknown;

  // first character of the string is the type of observation (for RINEX 3, C==P)
  auto ot = c2obstype (str[0],j);
  if (j) {
    if (str[0]=='P')
      ot = ObservationType::Range;
    else
      return rtn;
  }

  // make band an integer (or die trying...)
  int iband;
  try {
    iband = std::stoi (str.substr(1,1));
  } catch (...) {
    return rtn;
  }

  // mark all possible RINEX v2.12 satellite systems that are of interest (i.e. included
  // in the ss vector. Rinex v2.12 can have GPS, GLO, GAL, SBAS, BDS
  bool gps = true;
  bool glo = true;
  bool gal = true;
  bool sbs = true;
  bool bds = true;
  if (ss) {
    gps = ( std::find (ss->begin(),ss->end(),SatelliteSystem::Gps) != ss->end() );
    glo = ( std::find (ss->begin(),ss->end(),SatelliteSystem::Glonass) != ss->end() );
    gal = ( std::find (ss->begin(),ss->end(),SatelliteSystem::Galileo) != ss->end() );
    sbs = ( std::find (ss->begin(),ss->end(),SatelliteSystem::Sbas) != ss->end() );
    sbs = ( std::find (ss->begin(),ss->end(),SatelliteSystem::Bds) != ss->end() );
  }

  // there should be at least one satellite system of interest
  if ( !(gps+glo+gal+sbs+bds) ) return rtn;

  // match any possible RINEX v2.12 observable string
  switch (ot) {

    case (ObservationType::Range) :
      if (str[0] == 'C') {
        switch (iband) {
          case (1) ://___________________________________________C1________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,TrackingCode::C));     // GPS,C,L1,C
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,TrackingCode::C)); // GLO,C,G1,C
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E1,utc));             // GAL,C,E1,?
            if (sbs) rtn.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L1,TrackingCode::C));    // SBAS,C,L1,C
            break;
          case (2) ://___________________________________________C2________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,TrackingCode::C));     // GPS,C,L2,C
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,TrackingCode::C)); // GLO,C,G2,C
            break;
          case (5) ://___________________________________________C5________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L5,TrackingCode::C));     // GPS,C,L5,C
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,C,E5a,?
            if (sbs) rtn.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L5,TrackingCode::C));    // SBAS,C,L5,C
            break;
          case (7) ://___________________________________________C7________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,C,E5b,?
            break;
          case (8) ://___________________________________________C8________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5,utc));             // GAL,C,E5a+b,?
            break;
          case (6) ://___________________________________________C6________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E6,utc));             // GAL,C,E6,?
            break;
          default:
            break;
        }
      } else if (str[0] == 'P') {
        switch (iband) {
          case (1) ://___________________________________________P1________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,TrackingCode::P));     // GPS,C,L1,P
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,TrackingCode::P)); // GLO,C,G1,P
            break;
          case (2) ://___________________________________________P2________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,TrackingCode::P));     // GPS,C,L2,P
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,TrackingCode::P)); // GLO,C,G2,P
            break;
          default:
            break;
        }
      } else {
        return rtn;
      }
      break;

    case (ObservationType::Phase) :
      switch (iband) {
          case (1) ://___________________________________________L1________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,utc));                 // GPS,L,L1,?
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,utc));             // GLO,L,G1,?
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E1,utc));             // GAL,L,E1,?
            if (sbs) rtn.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L1,utc));                // SBAS,L,L1,?
            break;
          case (2) ://___________________________________________L2________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,utc));                 // GPS,L,L2,?
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,utc));             // GLO,L,G2,?
            break;
          case (5) ://___________________________________________L5________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L5,utc));                 // GPS,L,L5,?
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,L,E5a,?
            if (sbs) rtn.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L5,utc));                // SBAS,L,L5,?
            break;
          case (7) ://___________________________________________L7________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,L,E5b,?
            break;
          case (8) ://___________________________________________L8________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5,utc));             // GAL,L,E5a+b,?
            break;
          case (6) ://___________________________________________L6________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E6,utc));             // GAL,L,E6,?
            break;
          default:
            break;
      }
      break;

    case (ObservationType::Doppler) :
      switch (iband) {
          case (1) ://___________________________________________D1________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,utc));                 // GPS,D,L1,?
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,utc));             // GLO,D,G1,?
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E1,utc));             // GAL,D,E1,?
            if (sbs) rtn.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L1,utc));                // SBAS,D,L1,?
            break;
          case (2) ://___________________________________________D2________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,utc));                 // GPS,D,L2,?
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,utc));             // GLO,D,G2,?
            break;
          case (5) ://___________________________________________D5________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L5,utc));                 // GPS,D,L5,?
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,D,E5a,?
            if (sbs) rtn.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L5,utc));                // SBAS,D,L5,?
            break;
          case (7) ://___________________________________________D7________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,D,E5b,?
            break;
          case (8) ://___________________________________________D8________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5,utc));             // GAL,D,E5a+b,?
            break;
          case (6) ://___________________________________________D6________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E6,utc));             // GAL,D,E6,?
            break;
          default:
            break;
      }
      break;

    case (ObservationType::SNR) :
      switch (iband) {
          case (1) ://___________________________________________S1________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,utc));                 // GPS,S,L1,?
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,utc));             // GLO,S,G1,?
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E1,utc));             // GAL,S,E1,?
            if (sbs) rtn.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L1,utc));                // SBAS,S,L1,?
            break;
          case (2) ://___________________________________________S2________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,utc));                 // GPS,S,L2,?
            if (glo) rtn.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,utc));             // GLO,S,G2,?
            break;
          case (5) ://___________________________________________S5________________________________
            if (gps) rtn.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L5,utc));                 // GPS,S,L5,?
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,S,E5a,?
            if (sbs) rtn.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L5,utc));                // SBAS,S,L5,?
            break;
          case (7) ://___________________________________________S7________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,S,E5b,?
            break;
          case (8) ://___________________________________________S8________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5,utc));             // GAL,S,E5a+b,?
            break;
          case (6) ://___________________________________________S6________________________________
            if (gal) rtn.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E6,utc));             // GAL,S,E6,?
            break;
          default:
            break;
      }
      break;

    default : // No other observation type handled in RINEX v2.xx
      break;
      
  }// switch (ot)

  // return the vector
  return rtn;
  
}
