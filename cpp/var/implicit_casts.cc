#include <iostream>

class A {
private:
  int _a;

public:
  explicit A(int i = 0) noexcept : _a(i) {
    std::cout << "\n\t>Constructor for A called with argument: " << i;
  }
  A(const A &a) : _a(a._a) {
    std::cout << "\n\t>Copy constructor for A called";
  }
  A &operator=(const A &a) {
    std::cout << "\n\t>Assignment operator for A called";
    if (this != &a)
      _a = a._a;
    return *this;
  }
  A(A &&a) : _a(std::move(a._a)) {
    std::cout << "\n\t>Move constructor for A called";
  }
  A &operator=(A &&a) {
    std::cout << "\n\t>Move assignment operator for A called";
    _a = std::move(a._a);
    return *this;
  }
  int val() const noexcept { return _a; }
};

// only difference from A is a non-explicit constructor
class AA {
private:
  int _a;

public:
  AA(int i = 0) noexcept : _a(i) {
    std::cout << "\n\t>Constructor for AA called with argument: " << i;
  }
  AA(const AA &a) : _a(a._a) {
    std::cout << "\n\t>Copy constructor for AA called";
  }
  AA &operator=(const AA &a) {
    std::cout << "\n\t>Assignment operator for AA called";
    if (this != &a)
      _a = a._a;
    return *this;
  }
  AA(AA &&a) : _a(std::move(a._a)) {
    std::cout << "\n\t>Move constructor for AA called";
  }
  AA &operator=(AA &&a) {
    std::cout << "\n\t>Move assignment operator for AA called";
    _a = std::move(a._a);
    return *this;
  }
  int val() const noexcept { return _a; }
};

class B {
private:
  int _b;

public:
  explicit B(int i = 0) noexcept : _b(-i) {
    std::cout << "\n\t>Constructor for B called with argument: " << i;
  }
  int val() const noexcept { return _b; }
  // a B instance can be cast to A; note that this will allow:
  // A a1(b_instance);
  explicit operator A() const noexcept {
    std::cout << "\n\t>Casting instance B to an A instance";
    return A(-_b);
  }
  explicit operator AA() const noexcept {
    std::cout << "\n\t>Casting instance B to an AA instance";
    return AA(-_b);
  }
};

void foo(A a) { std::cout << "\nfoo called with A= " << a.val(); }
void foo(AA a) { std::cout << "\nfoo called with AA=" << a.val(); }

int main() {
  A a1(5);
  B b1(5);
  AA aa1;

  std::cout << "\na1 is " << a1.val() << "\nb1 is " << b1.val() << "\naa1 is "
            << aa1.val();

  A a2 = static_cast<A>(b1);
  std::cout << "\nI can cast a B instance to an A instance: e.g."
            << "\nA a2 = static_cast<A>(b1); will result in a2=" << a2.val();
  A a3 = A(b1);
  std::cout << "\nWe could also use a C-style cast, aka:"
            << "\nA a3 = A(b1); resulting in a3=" << a3.val();

  A a4(b1);
  std::cout << "\nThe cast B->A will be implicitely called to construct an A"
            << "\ninstance from a B instance:"
            << "\nA a4(b1); will result in a4=" << a4.val();

  // this is not allowed however!
  // A a5 = b1;
  // A a6 = 9; <-- Because of the 'explicit co'tor, this is illegal!!

  AA aa2 = static_cast<AA>(b1);
  std::cout << "\nI can cast a B instance to an AA instance: e.g."
            << "\nAA aa2 = static_cast<AA>(b1); will result in aa2="
            << aa2.val();
  AA aa3 = AA(b1);
  std::cout << "\nWe could also use a C-style cast, aka:"
            << "\nAA aa3 = AA(b1); resulting in aa3=" << aa3.val();

  AA aa4(b1);
  std::cout << "\nThe cast B->AA will be implicitely called to construct an AA"
            << "\ninstance from a B instance:"
            << "\nAA aa4(b1); will result in aa4=" << aa4.val();

  // this is not allowed however!
  // AA aa5 = b1;
  AA aa6 = 9; // <-- Non-'explicit co'tor, this is legal!!

  foo(a1);
  foo(aa1);
  // foo(b1); this won't compile!

  std::cout << "\n";
  return 0;
}
