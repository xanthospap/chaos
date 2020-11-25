#include <iostream>
#include <tuple>
#include <utility>

// --------------------------------------------------------------------------//
// Structured Bindings, c++17 (Part 2)
// --------------------------------------------------------------------------//

namespace A { // put that in a namespace to make things a bit more complicated!
class Class {
private:
  int i{};
  char c{};
  float f{};

public:
  Class() noexcept {}; // Empty con'tor; we do need this, see tuple_element<N,
                       // A::Class>
  Class(int _i, char _c, float _f) noexcept : i(_i), c(_c), f(_f){};
  // const case, i.e can do auto [i1,c1,f1] = a1;
  template <std::size_t N> decltype(auto) get() const {
    static_assert(N < 3);
    if constexpr (N == 0)
      return i;
    else if constexpr (N == 1)
      return c;
    else if constexpr (N == 2)
      return f;
  }
  // return references, so we can do auto& [ir,cr,fr] = a1
  // Note the parenthesis at the return statements and read this:
  // https://stackoverflow.com/questions/27557369/why-does-decltypeauto-return-a-reference-here
  template <std::size_t N> decltype(auto) get() {
    static_assert(N < 3);
    if constexpr (N == 0)
      return (i);
    else if constexpr (N == 1)
      return (c);
    else if constexpr (N == 2)
      return (f);
  }
};
} // namespace A

namespace std {
template <> struct tuple_size<A::Class> : integral_constant<size_t, 3> {};
/*
 * instead of these, templetize for any N, see: tuple_element<N, A::Class>
template<>
  struct std::tuple_element<0,A::Class> {using type = int;};
template<>
  struct std::tuple_element<1,A::Class> {using type = char;};
template<>
  struct std::tuple_element<2,A::Class> {using type = float;};
*/
template <size_t N>
// in case class A has no default constuctor, we need declval, see
// https://en.cppreference.com/w/cpp/utility/declval
// struct tuple_element<N, A::Class> {using
// type=decltype(std::declval<A::Class>().get<N>());}; else, declval is not
// needed
struct tuple_element<N, A::Class> {
  using type = decltype(A::Class().get<N>());
};
} // namespace std

using A::Class;
int main() {
  Class a1(1, 'b', 3.3);
  Class a2;
  auto [i1, c1, f1] = a1;
  std::cout << "\n" << i1 << ", " << c1 << ", " << f1;

  auto [i2, c2, f2] = a2;
  std::cout << "\n" << i2 << ", " << c2 << ", " << f2;

  auto &[ir, cr, fr] = a1;
  ir = 2;
  cr = 'a';
  fr = 4.4f;
  std::cout << "\n"
            << a1.get<0>() << ", " << a1.get<1>() << ", " << a1.get<2>();

  std::cout << "\n";
  return 0;
}
