#include <iostream>
#include <tuple>

class A {
public:
  int i{};
  float f{};
  std::string s{"empty"};
};

// generic print using fold expressions, version 1
template<typename T>
  const T& spaceBefore(const T& arg) {
    std::cout << " ";
    return arg;
}
template<typename First, typename... Args>
  void print(const First& firstarg, const Args&... args) {
    std::cout << firstarg;
    (std::cout << ... << spaceBefore(args)) << "\n";
}

// ... but why have two sperate functions? let's make the spaceBefore a lambda
// inside the print function
template<typename First, typename... Args>
  void printv2(const First& firstarg, const Args&... args) {
    std::cout << firstarg;
    auto spaceBefore = [](const auto& arg) {
      std::cout << " ";
      return arg;
    };
    (std::cout << ... << spaceBefore(args)) << "\n";
}

// However, note that by default, lambdas return objects by value, which means 
// that this would create an unnecessary copy of the passed argument. The way 
// to avoid that is to explicitly declare the return type of the lambda to be 
// const auto& or decltype(auto)
template<typename First, typename... Args>
  void printv3(const First& firstarg, const Args&... args) {
    std::cout << firstarg;
    auto spaceBefore = [](const auto& arg) -> const auto& {
      std::cout << " ";
      return arg;
    };
    (std::cout << ... << spaceBefore(args)) << "\n";
}

// ...well, fuck the seperate lambda! let's make combine this to one statement
template<typename First, typename... Args>
  void printv4(const First& firstarg, const Args&... args) {
    std::cout << firstarg;
    (std::cout << ... << [](const auto& arg) -> decltype(auto) {
      std::cout << " ";
      return arg;
    }(args)) << "\n";
}

// Nevertheless, a simpler way to implement print() is to use a lambda that 
// prints both the space and the argument and then pass this to a unary fold
template<typename First, typename... Args>
  void printv5(const First& firstarg, const Args&... args) {
    std::cout << firstarg;
    auto outWithSpace = [](const auto& arg) {
                          std::cout << " " << arg;
                        };
    (... , outWithSpace(args));
    std::cout<<"\n";
}

int main()
{
  A a1;
  print(a1.i, a1.f, a1.s);
  printv2(a1.i, a1.f, a1.s);
  printv3(a1.i, a1.f, a1.s);
  printv4(a1.i, a1.f, a1.s);
  printv5(a1.i, a1.f, a1.s);

  A a2{5, 5.5f, "non-empty"};
  print(a2.i, a2.f, a2.s);
  printv2(a2.i, a2.f, a2.s);
  printv3(a2.i, a2.f, a2.s);
  printv4(a2.i, a2.f, a2.s);
  printv5(a2.i, a2.f, a2.s);

  auto params = std::make_tuple(6, 6.6f, "tuple");
  std::apply(printv5, params);

  std::cout<<"\n";
  return 0;
}
