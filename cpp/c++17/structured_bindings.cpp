#include <cassert>
#include <iostream>
#include <map>
#include <string>

// --------------------------------------------------------------------------//
// Structured Bindings, c++17
// --------------------------------------------------------------------------//

struct A {
  int i{0};
  std::string s{"foo"};
  A(int ii, const std::string &ss) noexcept : i(ii), s(ss){};
};

struct B {
  int i{0};
  int v[3] = {1, 2, 3};
};

A ret_struct(int i, const std::string &str) noexcept { return A(i, str); }

int main() {
  A a1(10, "bar");

  //  bind members of the structure directly to new names
  auto [i1, s1](a1);
  assert(i1 == 10);
  assert(s1 == "bar");

  // assign the result to two entities directly giving local names to the
  // returned data members:
  auto [i2, s2] = ret_struct(5, "foo");
  assert(i2 == 5);
  assert(s2 == "foo");

  std::map<int, std::string> mymap{{1, "one"}, {2, "two"}, {3, "three"}};
  // way more intuitive std::map handling
  // To iterate over the elements of a std::map<> without structured bindings,
  // you would have to program:
  // for (const auto& elem : mymap) std::cout<<elem.first<<":
  // "<<elem.second<<'\n'; (note that the elements are std::pairs of the key and
  // value type) By using structured bindings, the code becomes a lot more
  // readable:
  for (const auto &[k, v] : mymap) {
    std::cout << "\n" << k << " -> " << v;
    assert(mymap[k] == v);
  }

  // unless references are used, modifying the value used for initialization
  // has no effect on the names initialized by a structured binding (and vice
  // versa):
  A a3{42, "hello"};
  auto [u1, v1] = a3;
  a3.i = 77;
  assert(u1 == 42);
  u1 = 99;
  assert(a3.i == 77);
  // but can bind to reference ....
  A a4{42, "hello"};
  auto &[u2, v2] = a4;
  a4.i = 77;
  assert(u2 == 77);
  u2 = 99;
  assert(a4.i == 99);

  // lets see about these arrays ....
  B b1;
  auto [bi1, bv1] = b1;
  assert(sizeof(bv1) == sizeof(b1.v));
  for (int i = 0; i < 3; i++)
    bv1[i] += 1;
  for (int i = 0; i < 3; i++)
    assert(bv1[i] == b1.v[i] + 1);
  auto &[bi2, bv2] =
      b1; // bv2 does not decay to pointer; it is a reference to the array
  assert(sizeof(bv2) == sizeof(b1.v));
  for (int i = 0; i < 3; i++)
    bv2[i] += 1;
  for (int i = 0; i < 3; i++)
    assert(bv2[i] == b1.v[i]);

  // another example .....
  double crd[3] = {10e0, 20e0, 3e0};
  auto &[x, y, z] = crd;
  x += .5e0;
  y += .5e0;
  z += .5e0;
  assert(crd[0] == 10.5e0);

  std::cout << "\nAll ok!\n";
  return 0;
}
