#include "gobst.hpp"

using namespace ngpt;
using std::string;

/**
 * @details Resolve an observable type, from a string formated as in RinexV2.x 
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
 * @param[in]  cstr  the string to be resolved, holding a RINEX v2.x observable 
 *                   identifier (e.g. 'L1') (\c string).
 * @param[out] obs   a vector containing all posibble observables the input string
 *                   can represent.
 * @param[in]  ss    (optional) if the user wants to limit the number of possible
 *                   observables that can be formed by the input string, then a
 *                   pointer to a vector can be given, containing the satellite
 *                   systems of interest.
 * @return           The number of possible observables resolved, i.e. the size
 *                   of \c obs vector.
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
@endverbatim
 *
 * @note The following table shows how Rinex V2.xx strings describing range observations 
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
 *
 * @note RINEX v2.xx, can contain only the satellite systems : GPS,Glonass,Sbas,Galileo
 *
 * Reference @cite rnx211
 *
 */
int raw_obs::resolve_v2x (const std::string& cstr,std::vector<raw_obs>& obs,
                          std::vector<SatelliteSystem>* ss)
{
  /* remove any trailing or leading whitespaces */
  string str = stripstr (cstr);

  /* pre-set status to ok */
  int /*status=0,*/ j;

  /* clear the vector */
  obs.clear();

  /* the size of the string must be equal to 2; else exit */
  if (str.size()!=2) return 0;

  /* cannot resolve tracking code either way */
  auto utc = TrackingCode::Unknown;

  /* first character of the string is the type of observation (for RINEX 3, C==P) */
  auto ot = c2obstype (str[0],j);
  if (j) {
    if (str[0]=='P')
      ot = ObservationType::Range;
    else
      return 0;
  }

  /* depending on the carrier and the observable type, find any possible combination */
  int iband;
  try {
    iband = std::stoi (str.substr(1,1));
  } catch (...) {
    return 0;
  }

  switch (ot) {

    case (ObservationType::Range) :
      if (str[0] == 'C') {
        switch (iband) {
          case (1) ://___________________________________________C1________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,TrackingCode::C));     // GPS,C,L1,C
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,TrackingCode::C)); // GLO,C,G1,C
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E1,utc));             // GAL,C,E1,?
            obs.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L1,TrackingCode::C));    // SBAS,C,L1,C
            return 4;
          case (2) ://___________________________________________C2________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,TrackingCode::C));     // GPS,C,L2,C
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,TrackingCode::C)); // GLO,C,G2,C
            return 2;
          case (5) ://___________________________________________C5________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L5,TrackingCode::C));     // GPS,C,L5,C
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,C,E5a,?
            obs.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L5,TrackingCode::C));    // SBAS,C,L5,C
            return 3;
          case (7) ://___________________________________________C7________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,C,E5b,?
            return 1;
          case (8) ://___________________________________________C8________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5,utc));             // GAL,C,E5a+b,?
            return 1;
          case (6) ://___________________________________________C6________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E6,utc));             // GAL,C,E6,?
            return 1;
          default:
            return 0;
        }
      } else if (str[0] == 'P') {
        switch (iband) {
          case (1) ://___________________________________________P1________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,TrackingCode::P));     // GPS,C,L1,P
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,TrackingCode::P)); // GLO,C,G1,P
            return 4;
          case (2) ://___________________________________________P2________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,TrackingCode::P));     // GPS,C,L2,P
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,TrackingCode::P)); // GLO,C,G2,P
            return 2;
          default:
            return 0;
        }
      } else {
        return 0;
      }

    case (ObservationType::Phase) :
      switch (iband) {
          case (1) ://___________________________________________L1________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,utc));                 // GPS,L,L1,?
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,utc));             // GLO,L,G1,?
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E1,utc));             // GAL,L,E1,?
            obs.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L1,utc));                // SBAS,L,L1,?
            return 4;
          case (2) ://___________________________________________L2________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,utc));                 // GPS,L,L2,?
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,utc));             // GLO,L,G2,?
            return 2;
          case (5) ://___________________________________________L5________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L5,utc));                 // GPS,L,L5,?
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,L,E5a,?
            obs.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L5,utc));                // SBAS,L,L5,?
            return 3;
          case (7) ://___________________________________________L7________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,L,E5b,?
            return 1;
          case (8) ://___________________________________________L8________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5,utc));             // GAL,L,E5a+b,?
            return 1;
          case (6) ://___________________________________________L6________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E6,utc));             // GAL,L,E6,?
            return 1;
          default:
            return 0;
      }

    case (ObservationType::Doppler) :
      switch (iband) {
          case (1) ://___________________________________________D1________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,utc));                 // GPS,D,L1,?
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,utc));             // GLO,D,G1,?
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E1,utc));             // GAL,D,E1,?
            obs.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L1,utc));                // SBAS,D,L1,?
            return 4;
          case (2) ://___________________________________________D2________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,utc));                 // GPS,D,L2,?
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,utc));             // GLO,D,G2,?
            return 2;
          case (5) ://___________________________________________D5________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L5,utc));                 // GPS,D,L5,?
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,D,E5a,?
            obs.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L5,utc));                // SBAS,D,L5,?
            return 3;
          case (7) ://___________________________________________D7________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,D,E5b,?
            return 1;
          case (8) ://___________________________________________D8________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5,utc));             // GAL,D,E5a+b,?
            return 1;
          case (6) ://___________________________________________D6________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E6,utc));             // GAL,D,E6,?
            return 1;
          default:
            return 0;
      }

    case (ObservationType::SNR) :
      switch (iband) {
          case (1) ://___________________________________________S1________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L1,utc));                 // GPS,S,L1,?
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G1,utc));             // GLO,S,G1,?
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E1,utc));             // GAL,S,E1,?
            obs.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L1,utc));                // SBAS,S,L1,?
            return 4;
          case (2) ://___________________________________________S2________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L2,utc));                 // GPS,S,L2,?
            obs.push_back (raw_obs (SatelliteSystem::Glonass,ot,CarrierBand::G2,utc));             // GLO,S,G2,?
            return 2;
          case (5) ://___________________________________________S5________________________________
            obs.push_back (raw_obs (SatelliteSystem::Gps,ot,CarrierBand::L5,utc));                 // GPS,S,L5,?
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,S,E5a,?
            obs.push_back (raw_obs (SatelliteSystem::Sbas,ot,CarrierBand::L5,utc));                // SBAS,S,L5,?
            return 3;
          case (7) ://___________________________________________S7________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5a,utc));            // GAL,S,E5b,?
            return 1;
          case (8) ://___________________________________________S8________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E5,utc));             // GAL,S,E5a+b,?
            return 1;
          case (6) ://___________________________________________S6________________________________
            obs.push_back (raw_obs (SatelliteSystem::Galileo,ot,CarrierBand::E6,utc));             // GAL,S,E6,?
            return 1;
          default:
            return 0;
      }

    default : /* No other observation type handled in RINEX v2.xx */
      return 0;
      
  }/* switch (ot) */
  
}
