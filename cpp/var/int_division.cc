#include <iostream>
#include <cstdio>
#include <cmath>
#include <chrono>

using namespace std::chrono;

constexpr long SEC_IN_DAY = 86400L;

long
sec2days__d1(double sec, double& sod) noexcept
{
  /* seconds to milliseconds for accuracy when transforming to long */
  long MSEC_IN_DAY = SEC_IN_DAY * 1e6L;
  long sec_long = static_cast<long>(sec*1e6);
  long days = sec_long / MSEC_IN_DAY;
  long sec_of_day = sec_long % MSEC_IN_DAY;
  sod = static_cast<double>(sec_of_day*1e-6);
  return days;
}

long
sec2days__d2(double sec, double& sod) noexcept
{
  sod = std::fmod(sec, SEC_IN_DAY*1e0);
  return static_cast<long>(sec) / SEC_IN_DAY;
}

int main()
{
  long days1, days2;
  double sod1, sod2;
  for (double sec=0.01; sec<SEC_IN_DAY*8.25; sec+=902e0) {
    days1 = sec2days__d1(sec, sod1);
    days2 = sec2days__d2(sec, sod2);
    if (std::abs(sod1-sod2)>1e-9 || days1!=days2) {
      printf("\n[ERROR] Seconds: %20.10f", sec);
      printf("\n        Alg1   : day: %03d and sod: %20.10f",(int)days1, sod1);
      printf("\n        Alg2   : day: %03d and sod: %20.10f",(int)days2, sod2);
    }
  }

  auto start = high_resolution_clock::now();
  for (double sec=0.01; sec<SEC_IN_DAY*8.25; sec+=92e0) days1 = sec2days__d1(sec, sod1);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  std::cout << "\nTime taken by function1: "<< duration.count() << " microseconds";
  
  start = high_resolution_clock::now();
  for (double sec=0.01; sec<SEC_IN_DAY*8.25; sec+=92e0) days1 = sec2days__d2(sec, sod1);
  stop = high_resolution_clock::now();
  duration = duration_cast<microseconds>(stop - start);
  std::cout << "\nTime taken by function2: "<< duration.count() << " microseconds";

  std::cout<<"\n";
  return 0;
}
