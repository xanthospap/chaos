#include "dates.hpp"
#include "codeaiub.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>

using std::string;

int main ()
{
  std::cout << "\n=============================================================";
  std::cout << "\nTEST ROUTINE FOR EXTERNAL LIBRARY";
  std::cout << "\n=============================================================\n";

  int status,flag,ok;
  string igs_name = "REYK";
  string bad_name = "_EYK";
  string antenna,receiver,remark;
  double n,e,u;
  bernsta sta ("/home/xanthos/Projects/chaos-ngp/new/chaos_documented/data/CODE.STA");
  datetime::date d (2014,10,1);

  printf ("\nStation name %5s",igs_name.c_str());
  // get type 002 information
  status = sta.getstainf (igs_name,d,receiver,antenna,n,e,u,flag,remark);
  printf ("\n\tType 002 status %01i",status);
  if (!status) printf ("\n\treceiver: %20s, antenna %20s, (dn,de,du)=(%+7.4f,%+7.4f,%+7.4f), flag: %01i, remark: %20s",
                      receiver.c_str(),antenna.c_str(),n,e,u,flag,remark.c_str());
  // get type 003 information
  status = sta.gettype3flag (igs_name,d,flag,remark);
  printf ("\n\tType 003 status %01i for %s",status,d.toymdstr(ok,0).c_str());
  if (!status) printf ("\n\tflag: %01i, remark: %20s",flag,remark.c_str());
  datetime::date d2 (2008,03,10);
  status = sta.gettype3flag (igs_name,d2,flag,remark);
  printf ("\n\tType 003 status %01i for %s",status,d2.toymdstr(ok,0).c_str());
  if (!status) printf ("\n\tflag: %01i, remark: %20s",flag,remark.c_str());  

  printf ("\nStation name %5s",bad_name.c_str());
  // get type 002 information
  status = sta.getstainf (bad_name,d,receiver,antenna,n,e,u,flag,remark);
  printf ("\n\tType 002 status %01i",status);
  if (!status) printf ("\n\treceiver: %20s, antenna %20s, (dn,de,du)=(%+7.4f,%+7.4f,%+7.4f), flag: %01i, remark: %20s",
                      receiver.c_str(),antenna.c_str(),n,e,u,flag,remark.c_str());
  // get type 003 information
  status = sta.gettype3flag (bad_name,d,flag,remark);
  printf ("\n\tType 003 status %01i",status);
  if (!status) printf ("\n\flag: %01i, remark: %20s",flag,remark.c_str());

  std::cout << "\n=============================================================";
  std::cout<<"\n";
  return 0;
}
