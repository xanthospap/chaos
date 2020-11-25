#include <iostream>
#include <limits>

/*
 * Let's say we have a value that is accompanied with a state; this state can
 * have 6 possible values, aka:
 * 1. absent
 * 2. clock_event
 * 3. clock_prediction
 * 4. maneuver
 * 5. orbit_prediction
 * 6. special
 * We want a class (as simple and small as possible) which will hold the
 * flag for this value. E.g., the value could be flagged with 1 and 5 or
 * any other combination. Obviously, this is the time for bitwise things!
 */
enum class state_attribute : unsigned char {
  absent = 0,
  clock_event = 1,
  clock_prediction = 2,
  maneuver = 3,
  orbit_prediction = 4,
  special = 5
};

class Flag
{
private:
  unsigned char bits_{0};
public:
  Flag() noexcept : bits_(0) {};

  Flag(state_attribute a) noexcept {
    bits_ |= (1 << static_cast<unsigned char>(a));
  };

  void
  set(state_attribute a) noexcept {
    bits_ |= (1 << static_cast<unsigned char>(a));
  };

  void
  clear(state_attribute a) noexcept {
    bits_ &= (~(1 << static_cast<unsigned char>(a)));
  };

  void
  represent() noexcept {
    std::cout<<"\nNumber is: "<<(unsigned int)bits_;
    int numbits = std::numeric_limits<unsigned char>::digits;
    std::cout<<"\nIn bits, that is: (# of bits:"<<numbits<<")";
    for (int i=0; i<numbits; i++) {
      std::cout<<"\n\tbit: "<<i<<" is "<< ((bits_ >> i) & 1);
    }
  }
};

int main()
{
  Flag f;
  f.set(state_attribute::clock_event);
  f.set(state_attribute::maneuver);
  f.represent();
  std::cout<<"\n";
  return 0;
}
