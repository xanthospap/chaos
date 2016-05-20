#include <type_traits>
#include <iostream>

//
// Given classes a, b and c, i want to make a new class d that can only be
// instatiated with a template parameter of a or b or c (i.e. d<a> should
// work, but d<int> should give a compile time error.
//

// Solution A: Use an (external) class (i.e. a traits class), and specialize
//             for a, b, and c.
/*
class a {};
class b {};
class c {};

template<class T> struct my_traits { constexpr static bool usable {false}; };
template<> struct my_traits<a> { constexpr static bool usable {true}; };
template<> struct my_traits<b> { constexpr static bool usable {true}; };
template<> struct my_traits<c> { constexpr static bool usable {true}; };

template<class T,
        typename = std::enable_if_t<my_traits<T>::usable>
        >
    class d {
private:
    T m;
public:
    d() : m{} {};
};

int main()
{
    d<a> da;
    d<b> db;
    // d<int> dint;

    return 0;
}
*/

// Solution B: In each of the classes (a, b and c), add a member variable and
//             rely on this during construction of d instances.

class a {
public:
    constexpr static bool usable {true};
};

class b {
public:
    constexpr static bool usable {true};
    explicit operator a() { return a{}; }
};

class c {
public:
    constexpr static bool usable {true};
    explicit operator b() { return b{}; }
    explicit operator a() { return a{}; }
};

template<class T,
        typename = std::enable_if_t<T::usable>
        >
    class d {
private:
    T m;
public:
    d() : m{} {};

    /// An instance can be instantiated using a parameter of type S, if:
    /// a) S has a member variable ::usable == true and
    /// b) an instance of type S can be cast to an instance of type T
    template<class S,
            typename = std::enable_if_t<T::usable>,
            typename = std::enable_if_t<
                std::is_same<T, decltype(static_cast<T>(S{}))>::value,
                bool
                >
            >
    d(S s) : m{} {};
};

int main()
{
    a a1;
    b b1;
    c c1;

    d<a> da;
    d<b> db;
    // d<int> dint; ERROR
    
    // A c instance can be cast to an a instance so the following should work
    d<a> dac{c1};
    // An a instance though cannot be cast to a c instance, so the following
    // should fail!
    // d<c> dca{a1}; ERROR

    return 0;
}
