#ifndef _OBS_RINEX_HPP
#define _OBS_RINEX_HPP

#include <utility>
#include "rinex.hpp"
#include "gobst.hpp"
#include "gsats.hpp"
#include "gsite.hpp"

typedef std::vector<std::pair<ngpt::SatelliteSystem,double> > td_pairvec;
typedef std::vector<std::pair<ngpt::Observable,int> > td_oipair;
typedef std::vector<std::pair<ngpt::SatelliteSystem,std::string> > td_cappvec;
/**
 * @brief This structure is used to hold GPS satellite-specific L1,L2 Wavelength
 *        factors. Its element is a pair of <Satellite,l1_l2_wlf> , where l1_l2_wlf
 *        is also a pair of <l1_wlf,l2_wlf> i.e GPS L1 carrier wavelength factor, 
 *        GPS L2 carrier wavelength factor.
 */
typedef std::vector<std::pair<ngpt::Satellite,std::pair<int,int> > > td_satint;

namespace ngpt {

/**
 * @details A class to hold Observation RINEX files (input). Can handle RINEX
 *          versions: 1.xx, 2.xx and 3.xx
 * References @cite rnx302 , @cite rnx211
 * @todo
 * - Need code to handle Rinex v2.12, including a raw_obs::resolve_v212 function
 * - Rinex v3.xx not fully handled yet.
 */
class ObsRinex : public Rinex {

  public:

  /**
   * @brief Valid Observation Rinex versions
   */
  enum class VERSION : unsigned char {
    Unknown,           ///< Unknown
    v100,              ///< Version 1
    v200, v210, v211,  ///< Version 2
    v300, v301, v302   ///< Version 3
  };

    /**
     * @details Constructor from Observation Rinex name; On initialization, the RINEX's 
     *          first line is read and the Version and Type are resolved. If this fails,
     *          the version is set to VERSION::Unknown and an error message is issued.
     */
    explicit ObsRinex (const char* c) : Rinex( std::string (c) ),mversion(VERSION::Unknown),
            msatsys(ngpt::SatelliteSystem::Unknown),mrcvclkappl(false),mdatastart(0),
            msquaring(false),ml12_wlf(nullptr),msat_wlf(nullptr)
            {this->resolve_version ();}

    /**
     * @details Constructor from Observation Rinex name; On initialization, the RINEX's 
     *          first line is read and the Version and Type are resolved. If this fails,
     *          the version is set to VERSION::Unknown and an error message is issued.
     */
    explicit ObsRinex (const std::string& s) : Rinex(s),mversion(VERSION::Unknown),
            msatsys(ngpt::SatelliteSystem::Unknown),mrcvclkappl(false),mdatastart(0),
            msquaring(false),ml12_wlf(nullptr),msat_wlf(nullptr)
            {this->resolve_version ();}

    /*! Destructor */
    ~ObsRinex() {
      if (ml12_wlf) delete ml12_wlf;
      if (msat_wlf) delete msat_wlf;
    }

    /*! Read the RINEX's header */
    inline int readheader () {
      switch (mversion) {
        case VERSION::v100 :
        case VERSION::v200 :
        case VERSION::v210 :
        case VERSION::v211 : return readheader_v2x ();
        case VERSION::v300 :
        case VERSION::v301 :
        case VERSION::v302 : return readheader_v3x ();
        default: return 1;
      }
    }

  private:

    /*! Read a RINEX header for Versions v2.XX */
    int readheader_v2x ();

    /*! Read a RINEX header for Versions v3.XX */
    int readheader_v3x ();

    /*! Resolve RINEX's version and verify type */
    int resolve_version ();

    VERSION                 mversion;    ///< Enum type for version; line 1 of header
    ngpt::SatelliteSystem   msatsys;     ///< The files' satellite system; line 1 of header
    ngpt::Site              msite;       ///< The site described in this Rinex
    td_pairvec              mscalefac;   ///< Scale factors for each sat. system (see @cite rnx302 Sec. 5.5)
    size_t                  mstartbit;   ///< Marks start of observations in the RINEX file
    int                     mnumofobsv;  ///< Original number of observables, as written in Rinex file. Note that this may be different from the size of the mobsv for RINEX v < 3.00
    std::vector<Observable> mobsv;       ///< List of observables, @see mnumofobsv
    td_cappvec              mdcbapplied; ///< If dcb's are applied, then this will hold pairs of <sat. system,correction_file> (see @cite rnx302 Sec. 5.14)
    td_cappvec              mpcvapplied; ///< If pcv are applied, then this will hold pairs of <sat. system,correction_file> (see @cite rnx302 Sec. 5.15)
    td_oipair               mscalefactor;///< Scale factor for every observable; if empty, all are equal to 1
    bool                    mrcvclkappl; ///< True if receiver-derived clock offset are applied to RINEX data
    size_t                  mdatastart;  ///< The start bit of observations in the RINEX.
    bool                    msquaring;   ///< This is aflag to represent that the RINEX has WAVELENGTH FACTORS
    std::vector<int>*       ml12_wlf;    ///< If msquaring is true, then this will hold the wavelength factors for L1 and L2 (Gps-specific)
    td_satint*              msat_wlf;    ///< If msquaring is true, then this will hold the wavelength factors for each satellite (Gps-specific)

}; /* end Rinex class */

}; /* end of namespace */

#endif
