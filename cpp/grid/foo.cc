#include <iostream>
#include <functional>
#include <type_traits>

/*
enum class Enum : char { e1, e2 };

template<Enum E>
    int foo(int i) noexcept { return i;}
template<>
    int foo<Enum::e1>(int i) noexcept { return i+1; }
template<>
    int foo<Enum::e2>(int i) noexcept { return i+2; }

template<Enum E>
    class C
{
public:
    // approach 1
    int bar(int i) const noexcept { return foo<E>(i); }
    // aproach 2
    static constexpr auto& bar2 = foo<E>;
};

int main()
{
    C<Enum::e1> c1;
    C<Enum::e2> c2;

    std::cout<<c1.bar(0)<<"\n";
    std::cout<<c2.bar(0)<<"\n";
    
    std::cout<<c1.bar2(0)<<"\n";
    std::cout<<c2.bar2(0)<<"\n";

    return 0;
}
*/

enum class Enum : char { e1, e2 };

template<Enum E>
    int foo(int i, int j) noexcept { return i+j;}
template<>
    int foo<Enum::e1>(int i, int j) noexcept { return i+1+j; }
template<>
    int foo<Enum::e2>(int i, int j) noexcept { return i+2+j; }

template<Enum E>
    class C
{
public:
    C(int i=0):j(i){};
    // approach 1
    int bar(int i) const noexcept { return foo<E>(i,j); }
    // aproach 2
    std::function<int(int)> bar2 = std::bind(foo<E>, std::placeholders::_1, std::cref(j));
    std::function<int(int)> bar3 = std::bind(foo<E>, std::placeholders::_1, baz());
    // approach 3
    int bar4(int i) noexcept;
    // check if bar4 can be used within the class decleration.
    int bar4_wrapper(int i) { return this->bar4(i); }
private:
    int j;
    int baz() const { 
        std::cout<<"\tBaz called!\n";
        return j; 
    }
};
template<> int C<Enum::e1>::bar4(int i) noexcept { return i+1+j; }
template<> int C<Enum::e2>::bar4(int i) noexcept { return i+2+j; }

template<typename T, Enum E>
    class D
{
public:
    D(T i):j{i}{}
    T bar(T k) noexcept { return bar_impl(k, enum_type()); }
private:
    typedef std::integral_constant<Enum, Enum::e1> e_e1;
    typedef std::integral_constant<Enum, Enum::e2> e_e2;
    typedef std::integral_constant<Enum, E>        e_e3;
    using enum_type = std::is_same<e_e1, e_e3>;

    T bar_impl(T k, std::true_type) noexcept  { return k+1+j; }
    T bar_impl(T k, std::false_type) noexcept { return k+2+j; }
    T j;
};

int main()
{
    C<Enum::e1> c1;
    C<Enum::e2> c2;

    std::cout<<c1.bar(0)<<"\n";
    std::cout<<c2.bar(0)<<"\n";
    
    std::cout<<c1.bar2(0)<<"\n";
    std::cout<<c2.bar2(0)<<"\n";
    
    std::cout<<c1.bar3(0)<<"\n";
    std::cout<<c2.bar3(0)<<"\n";
    
    std::cout<<c1.bar4(0)<<"\n";
    std::cout<<c2.bar4(0)<<"\n";

    std::cout<<c1.bar4_wrapper(0)<<"\n";
    std::cout<<c2.bar4_wrapper(0)<<"\n";
    
    D<int, Enum::e1> d1(0);
    D<int, Enum::e2> d2(0);
    std::cout<<"\tD: "<<d1.bar(0)<<"\n";
    std::cout<<"\tD: "<<d2.bar(0)<<"\n";
    
    return 0;
}
