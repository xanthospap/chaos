#include <iostream>
#include "varutils.hpp"

using std::vector;
using std::string;
using std::cout;

// Compilation: g++ -Wall -std=c++11 -I../../inc/ -L../../lib/ test_varutils.cpp -o test_vutils.e -lvutils0.0-5beta

int main ()
{

  // Example usage of function splitstr
  string s1 = "Akoko1Akoko2AAkoko3AAAkoko4AAA";
  vector<string> sv = splitstr (s1,'A');
  for (auto i : sv)
    cout<<"["<<i<<"]";
  cout << std::endl;

  // Example usage of function stripstr
  vector<string> strvec;
  strvec.push_back ("AAA   koko           A");
  strvec.push_back ("koko           AAA");
  strvec.push_back ("AAA   koko");
  strvec.push_back ("koko");
  strvec.push_back ("AAAAAAAAAAAAAAAAA");
  strvec.push_back ("A");
  strvec.push_back ("");
  for (auto x : strvec)
    cout << "\nOriginal string ["<<x<<"] converted to ["<<stripstr (x,'A')<<"]";
  cout << std::endl;

  // Example usage of function fortran2cpp_sci
  vector<string> strv;
  strv.push_back (" 1.2345D ");
  strv.push_back ("12345D2");
  strv.push_back (" .012345d3");
  strv.push_back ("12.345D0");
  strv.push_back ("12345D-3");
  strv.push_back ("1.2345.D-3");
  strv.push_back ("1.2t345D-3");
  for (auto& x : strv) {
    bool ok,check=1;
    string s = fortran2cpp_sci (x,ok,check);
    printf ("\nString %10s becomes %10s , ok=%01i, check=%01i",x.c_str (),s.c_str (),ok,check);
  }
  cout << std::endl;

  cout << std::endl;
  return 0;
}

// Expected output :
//
//[koko1][koko2][koko3][koko4]
//
//Original string [AAA   koko           A] converted to [AAA   koko           A]
//Original string [koko           AAA] converted to [koko           AAA]
//Original string [AAA   koko] converted to [AAA   koko]
//Original string [koko] converted to [koko]
//Original string [AAAAAAAAAAAAAAAAA] converted to [AAAAAAAAAAAAAAAAA]
//Original string [A] converted to [A]
//Original string [] converted to []
//
//String   1.2345D  becomes    1.2345E , ok=1, check=1
//String    12345D2 becomes    12345E2 , ok=1, check=1
//String  .012345d3 becomes  .012345E3 , ok=1, check=1
//String   12.345D0 becomes   12.345E0 , ok=1, check=1
//String   12345D-3 becomes   12345E-3 , ok=1, check=1
//String 1.2345.D-3 becomes 1.2345.D-3 , ok=0, check=1
//String 1.2t345D-3 becomes 1.2t345D-3 , ok=0, check=1
//
