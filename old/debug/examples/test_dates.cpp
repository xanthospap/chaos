#include "dates.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>

// Compilation g++ -Wall -std=c++11 -DDT_PRC_NANO -I../../inc/ -L../../lib/ test_dates.cpp \
// -o test_dates.e -ldatetime0.0-5beta -lvutils0.0-5beta

using std::vector;
using std::string;
using std::cout;
using namespace datetime;

int main ()
{
  const double SEC_PRC_ = datetime_precision_secs ();
  const double JD_PRC_  = datetime_precision_days ();

  cout << "\n=============================================================";
  cout << "\nTEST ROUTINE FOR DATETIME LIBRARY";
  cout << "\nCompiled with accuracy: "; printf ("%e",SEC_PRC_);
  cout << "\n=============================================================\n";
  int status;


  // TEST [A] cal2jd and dtf2d
  // test the cal2jd and dtf2d routines; these are changed to produce MJD and not JD
  // as the original SOFA routines
  cout << "\n-------------------------------------------------------------";
  cout << "\nA. testing modified routines cal2jd and dtf2d against the original ones";
  double tt1,tt2,st1,st2;
  status=0;
  cal2jd(2014,10,13,tt1,tt2);
  printf ("\ncal2jd ngp:  %17.10f,%17.10f",tt1,tt2);
  cal2jd_(2014,10,13,st1,st2);
  printf ("\ncal2jd sofa: %17.10f,%17.10f",st1,st2);
  printf ("\nDifference in resulting JD : %17.15f",(st1+st2)-(tt1+tt2+DJM0));
  if (fabs((st1+st2)-(tt1+tt2+DJM0))>JD_PRC_) {
    printf ("\nERROR! Routine cal2jd failed! ERROR %e",fabs((st1+st2)-(tt1+tt2+DJM0)));
    status = 1;
  }
  dtf2d("UTC",2014,10,13,23,59,59.0,tt1,tt2);
  printf ("\ndtf2d ngp:  %17.10f,%17.10f",tt1,tt2);
  dtf2d_("UTC",2014,10,13,23,59,59.0,st1,st2);
  printf ("\ndtf2d sofa: %17.10f,%17.10f",st1,st2);
  printf ("\nDifference in resulting JD : %17.15f",(st1+st2)-(tt1+tt2+DJM0));
  if (fabs((st1+st2)-(tt1+tt2+DJM0))>JD_PRC_) {
    printf ("\nERROR! Routine dtf2d failed!ERROR %e",fabs((st1+st2)-(tt1+tt2+DJM0)));
    status += 1;
  }
  cout << "\nSTATUS: " << ((status) ? "FAILED" : "SUCCESS");
  cout << "\n-------------------------------------------------------------\n";


  // TEST [B] reading / writting dates
  cout << "\n-------------------------------------------------------------";
  cout << "\nB. testing the reading and writting of date class";
  vector<string> strdates;
  strdates.push_back( "2010 12 13 22 30 0.1");              //[00]
  strdates.push_back( "2010 12 13 22 30 0.01");             //[01]
  strdates.push_back( "2010 12 13 22 30 0.001");            //[02] DT_PRC_MILLI
  strdates.push_back( "2010 12 13 22 30 0.0001");           //[03]
  strdates.push_back( "2010 12 13 22 30 0.00001");          //[04]
  strdates.push_back( "2010 12 13 22 30 0.000001");         //[05] DT_PRC_MICRO
  strdates.push_back( "2010 12 13 22 30 0.0000001");        //[06]
  strdates.push_back( "2010 12 13 22 30 0.00000001");       //[07]
  strdates.push_back( "2010 12 13 22 30 0.000000001");      //[08] DT_PRC_NANO
  strdates.push_back( "2010 12 13 22 30 0.0000000001");     //[09]
  strdates.push_back( "2010 12 13 22 30 0.00000000001");    //[10]
  strdates.push_back( "2010 12 13 22 30 0.000000000001");   //[11] DT_PRC_PICO
  strdates.push_back( "2010 12 13 22 30 0.0000000000001");  //[12] 
  strdates.push_back( "2010 12 13 22 30 0.00000000000001"); //[13] 
  strdates.push_back( "2010 12 13 22 30 0.000000000000001");//[14] DT_PRC_ 1.0e-15 SECOND (!!)

  vector<Date> dates;
  int ok, size=strdates.size(), j=0;
  double pre = 1e0;
  for (int i=0;i<size;i++) {
    j++;
    pre *= 1e-1;
    Date d;
    ok = d.fromymdstr (strdates[i]);
    if (ok) std::cerr<<"CORRUPTED DATE!!!";
    dates.push_back (d);
    if (SEC_PRC_==1e-03)
      printf ("\n[%02i] given date %35s, sec of day %17.4f precision: %e",i,d.toymdstr (ok,j).c_str (),d.secofday (),pre);
    else if (SEC_PRC_==1e-06)
      printf ("\n[%02i] given date %35s, sec of day %17.7f precision: %e",i,d.toymdstr (ok,j).c_str (),d.secofday (),pre);
    else if (SEC_PRC_==1e-09)
      printf ("\n[%02i] given date %35s, sec of day %17.10f precision: %e",i,d.toymdstr (ok,j).c_str (),d.secofday (),pre);
    else if (SEC_PRC_==1e-12)
      printf ("\n[%02i] given date %35s, sec of day %17.13f precision: %e",i,d.toymdstr (ok,j).c_str (),d.secofday (),pre);
    if (ok) std::cerr<<"CORRUPTED DATE!!!";
  }
  for (int i=1;i<size;i++) {
    double dt = dates[i-1].deltasec (dates[i]);
    bool bl;
    if (SEC_PRC_==1e-03)
      printf ("\nDifference between %2i and %2i is %15.4f. Are they the same? %1i",i,i-1,dt,bl=(dates[i]==dates[i-1]));
    else if (SEC_PRC_==1e-06)
      printf ("\nDifference between %2i and %2i is %15.7f. Are they the same? %1i",i,i-1,dt,bl=(dates[i]==dates[i-1]));
    else if (SEC_PRC_==1e-09)
      printf ("\nDifference between %2i and %2i is %15.10f. Are they the same? %1i",i,i-1,dt,bl=(dates[i]==dates[i-1]));
    else if (SEC_PRC_==1e-12)
      printf ("\nDifference between %2i and %2i is %15.13f. Are they the same? %1i",i,i-1,dt,bl=(dates[i]==dates[i-1]));
  }

  // TEST [C] adding / subtracking seconds
  cout << "\n\n-------------------------------------------------------------";
  cout << "\nC. testing the addition and subtraction of seconds";
  Date D (2014,10,13,0,0,0.0e0);
  Date E (2014,10,14,0,0,0.0e0);
  Date F (D);
  Date G (D);
  for (int i=0;i<86400;i+=30){
    D.addsec (30.0e0);
  }
  F.addsec (86400.0e0);
  printf ("\n(a) original date                           : %s",G.toymdstr (ok,9).c_str ());
  printf ("\n(b) sequentialy adding 30 sec up to next day: %s",D.toymdstr (ok,9).c_str ());
  printf ("\n(c) adding at once 86400 secs               : %s",F.toymdstr (ok,9).c_str ());
  printf ("\ndifference between (a) and (b)              : %15.9f (%s-%s)",D.deltasec (G),D.toymdstr (ok,9).c_str (),G.toymdstr (ok,9).c_str ());
  printf ("\ndifference between (a) and (c)              : %15.9f (%s-%s)",D.deltasec (F),D.toymdstr (ok,9).c_str (),F.toymdstr (ok,9).c_str ());
  printf ("\ndifference between (b) and (c)              : %15.9f (%s-%s)",G.deltasec (F),G.toymdstr (ok,9).c_str (),F.toymdstr (ok,9).c_str ());
  printf ("\napproximate error from sequentialy adding 30 sec is %010.3f nanosec.",D.secofday ()*1e09);

  cout<< "\n=============================================================";
  cout<<"\n";
  return 0;
}

// Expected output :
/*
=============================================================
TEST ROUTINE FOR DATETIME LIBRARY
Compiled with accuracy: 1.000000e-09
=============================================================

-------------------------------------------------------------
A. testing modified routines cal2jd and dtf2d against the original ones
cal2jd ngp:   56943.0000000000,     0.0000000000
cal2jd sofa: 2400000.5000000000, 56943.0000000000
Difference in resulting JD : 0.000000000000000
dtf2d ngp:   56943.0000000000,     0.9999884259
dtf2d sofa: 2456943.5000000000,     0.9999884259
Difference in resulting JD : 0.000000000000000
STATUS: SUCCESS
-------------------------------------------------------------

-------------------------------------------------------------
B. testing the reading and writting of date class
[00] given date               2010/12/13/22:30:00.1, sec of day  81000.1000000000 precision: 1.000000e-01
[01] given date              2010/12/13/22:30:00.01, sec of day  81000.0100000000 precision: 1.000000e-02
[02] given date             2010/12/13/22:30:00.001, sec of day  81000.0010000000 precision: 1.000000e-03
[03] given date            2010/12/13/22:30:00.0001, sec of day  81000.0001000000 precision: 1.000000e-04
[04] given date           2010/12/13/22:30:00.00001, sec of day  81000.0000100000 precision: 1.000000e-05
[05] given date          2010/12/13/22:30:00.000001, sec of day  81000.0000010000 precision: 1.000000e-06
[06] given date         2010/12/13/22:30:00.0000001, sec of day  81000.0000001000 precision: 1.000000e-07
[07] given date        2010/12/13/22:30:00.00000001, sec of day  81000.0000000100 precision: 1.000000e-08
[08] given date       2010/12/13/22:30:00.000000001, sec of day  81000.0000000010 precision: 1.000000e-09
[09] given date      2010/12/13/22:30:00.0000000000, sec of day  81000.0000000001 precision: 1.000000e-10
[10] given date     2010/12/13/22:30:00.00000000000, sec of day  81000.0000000000 precision: 1.000000e-11
[11] given date    2010/12/13/22:30:00.000000000000, sec of day  81000.0000000000 precision: 1.000000e-12
[12] given date   2010/12/13/22:30:00.0000000000000, sec of day  81000.0000000000 precision: 1.000000e-13
[13] given date  2010/12/13/22:30:00.00000000000000, sec of day  81000.0000000000 precision: 1.000000e-14
[14] given date 2010/12/13/22:30:00.000000000000000, sec of day  81000.0000000000 precision: 1.000000e-15
Difference between  1 and  0 is    0.0900000000. Are they the same? 0
Difference between  2 and  1 is    0.0090000000. Are they the same? 0
Difference between  3 and  2 is    0.0009000000. Are they the same? 0
Difference between  4 and  3 is    0.0000900000. Are they the same? 0
Difference between  5 and  4 is    0.0000090000. Are they the same? 0
Difference between  6 and  5 is    0.0000009000. Are they the same? 0
Difference between  7 and  6 is    0.0000000900. Are they the same? 0
Difference between  8 and  7 is    0.0000000090. Are they the same? 0
Difference between  9 and  8 is    0.0000000009. Are they the same? 0
Difference between 10 and  9 is    0.0000000001. Are they the same? 1
Difference between 11 and 10 is    0.0000000000. Are they the same? 1
Difference between 12 and 11 is    0.0000000000. Are they the same? 1
Difference between 13 and 12 is    0.0000000000. Are they the same? 1
Difference between 14 and 13 is    0.0000000000. Are they the same? 1

-------------------------------------------------------------
C. testing the addition and subtraction of seconds
(a) original date                           : 2014/10/13/00:00:00.000000000
(b) sequentialy adding 30 sec up to next day: 2014/10/14/00:00:00.000000007
(c) adding at once 86400 secs               : 2014/10/14/00:00:00.000000000
difference between (a) and (b)              : 86400.000000007 (2014/10/14/00:00:00.000000007-2014/10/13/00:00:00.000000000)
difference between (a) and (c)              :     0.000000007 (2014/10/14/00:00:00.000000007-2014/10/14/00:00:00.000000000)
difference between (b) and (c)              : -86400.000000000 (2014/10/13/00:00:00.000000000-2014/10/14/00:00:00.000000000)
approximate error from sequentialy adding 30 sec is 000006.600 nanosec.
=============================================================

*/
