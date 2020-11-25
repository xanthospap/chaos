#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

using namespace std::chrono;

#if defined(__GNUC__) && !defined(__llvm__)
constexpr double PI{std::atan(1e0) * 4e0};
#else
#define _USE_MATH_DEFINES
constexpr double PI{M_PI};
#endif

constexpr double lower = -PI;
constexpr double upper = PI;

// see
// https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
template <typename TReal>
bool approxEqual(
    TReal a, TReal b,
    TReal tolerance = std::numeric_limits<TReal>::epsilon()) noexcept {
  TReal diff = std::abs(a - b);
  if (diff < tolerance)
    return true;
  if (diff < std::max(std::abs(a), std::abs(b)) * tolerance)
    return true;
  return false;
}

// compute trig numbers (sin and cos) of angle theta using a trick
// from Eq. 5.4.8, in Numerical recipes 2007
void sincos(double theta, double &sint, double& cost) noexcept {
  const double t = std::tan(theta/2e0);
  const double t2 = t*t;
  const double t2p1 = t2 + 1e0;
  cost = (1e0-t2)/t2p1;
  sint = 2e0*t / t2p1;
  return;
}

double generate_random_double(double lower_bound, double upper_bound) noexcept {
  std::random_device rd;
  std::mt19937 e2(rd());
  std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
  return unif(e2);
}

auto time_trick_function(int samples) noexcept {
  double angle, sint, cost;
  auto start = high_resolution_clock::now();
  for (int i=0; i<samples; i++) {
    angle = generate_random_double(lower, upper);
    sincos(angle, sint, cost);
  }
  auto stop = high_resolution_clock::now();
  return duration_cast<microseconds>(stop - start);
}

auto time_trig_function(int samples) noexcept {
  double angle, sint, cost;
  auto start = high_resolution_clock::now();
  for (int i=0; i<samples; i++) {
    angle = generate_random_double(lower, upper);
    sint = std::sin(angle);
    cost = std::cos(angle);
  }
  auto stop = high_resolution_clock::now();
  return duration_cast<microseconds>(stop - start);
}

double rad2deg(double angle) noexcept { return angle * 180e0 / PI; }

int main() {

  // first of all, lets check if the trick in Numerical Recipes gives correct
  // results
  double angle, sina, cosa;
  int sin_probs=0, cos_probs=0, samples=500000;
  for (int i=0; i<samples; i++) {
    angle = generate_random_double(-2e0*PI, 2e0*PI);
    sincos(angle, sina, cosa);
    if (!approxEqual(sina, std::sin(angle))) {
      printf("\n[WARNING] sin diff larger than tolerance: %20.15f for angle %20.15f (#%5d)", rad2deg(sina-std::sin(angle)), rad2deg(angle), sin_probs+1);
      ++sin_probs;
    }
    if (!approxEqual(cosa, std::cos(angle))) {
      printf("\n[WARNING] cos diff larger than tolerance: %20.15f for angle %20.15f (#%5d)", rad2deg(cosa-std::cos(angle)), rad2deg(angle), cos_probs+1);
      ++cos_probs;
    }
  }
  printf("\nTested %10d angles; difference larger than tollerance: sin %5.1f%% cos %5.1f%%", samples, cos_probs*100e0/samples, sin_probs*100e0/samples);
  // test extreme cases
  std::vector<double> xtreme_cases = {0e0, -2e0*PI, -PI, PI, 2e0*PI};
  for (auto a : xtreme_cases) {
    sincos(a, sina, cosa);
    printf("\nTesting extreme angle: %15.5f", rad2deg(a));
    if (!approxEqual(sina, std::sin(a))) {
      printf("\n[WARNING] sin diff larger than tolerance: %20.15f for angle %20.15f", rad2deg(sina-std::sin(a)), rad2deg(a));
    }
    if (!approxEqual(cosa, std::cos(a))) {
      printf("\n[WARNING] cos diff larger than tolerance: %20.15f for angle %20.15f", rad2deg(cosa-std::cos(a)), rad2deg(a));
    }
  }

  /*
  std::size_t count_normal ,count_trick;
  int inits=5;
  for (int s=inits; s<100; s+= (s<100?3:10)) {
    count_trick = count_normal = 0;
    for (int t=0; t<5; t++) {
      auto t_normal = time_trig_function(s);
      count_normal += t_normal.count();
    }
    for (int t=0; t<5; t++) {
      auto t_trick = time_trick_function(s);
      count_trick += t_trick.count();
    }
    count_trick /= 5;
    count_normal /= 5;
    printf("\nnormal %5ld trick %5ld size %7d", count_normal, count_trick, s);
  }
  */

  return 0;
}
