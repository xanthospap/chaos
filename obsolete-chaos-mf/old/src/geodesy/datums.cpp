#include "datums.hpp"
#include <string.h>

/** 
 * @details default constructor makes GRS80.
 */
geo::Ellipsoid::Ellipsoid ()
  :a(6378137.0e0),
  f(0.003352810681225),
  name("GRS80")
{
};

/**
 * @details construct using a valid name; valid names are: <br> <tt>
 *          GRS80 <br>
 *          ITRF  <br>
 *          WGS84 <br>
 *          PZ90  <br> </tt>
 *          If the given string does not match any of the valid ellispoid names,
 *          then the default ellipsoid GRS80 is initialized.
 *
 * @param[in] n the name of the ellipsoid (\c char *)
 *
 */
geo::Ellipsoid::Ellipsoid (const char* n)
{
  /* search for ellipsoid name */
  if (!strcmp(n,"GRS80")) {
    a = 6378137.0;
    f = 1.0e00/298.25722210088;
    name=n;
  } else if (!strcmp(n,"ITRF")) {
    a = 6378137.0;
    f = 1.0e00/298.25722210088;
    name=n;
  } else if (!strcmp(n,"WGS84")) {
    a = 6378137.0;
    f = 1.0e00/298.257223563;
    name=n;
  } else if (!strcmp(n,"PZ90")) {
    a = 6378137.0;
    f = 1.0e00/298.257223563;
    name=n;
  } else {
    /* name is not valid! */
    //std::cerr<<"\n*** Unknown Ellipsoid/datum: "<<n;
    //std::cerr<<"\n    Making default Ellipsoid GRS80";
    a = 6378137.0;
    f = 1.0e00/298.25722210088;
    name="GRS80";
  }
}

/** 
 * @details construct using geometric values (i.e. construct a user-defined
 *          ellispoid). The name is set to 'USER-DEFINED'.
 *
 */
geo::Ellipsoid::Ellipsoid(double& aa,double& ff):
  a(aa),f(ff)
{
  name="USER-DEFINED";
}
