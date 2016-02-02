#include "obsrnx.hpp"
#include <stdio.h>

int main (int argc, char* argv[])
{
  if (argc < 2) {
    printf ("\nMust provide rinex name!\n");
    return 1;
  }

  std::string filename = argv[1];

  ngpt::ObsRinex rnx (filename);
  int ok = rnx.readheader ();

  printf ("\nReading Rinex header, returned: %1i",ok);

  printf ("\n");
  return 0;
}
