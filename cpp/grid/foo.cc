#include <iostream>

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
#include <functional>

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
private:
    int j;
    int baz() const { 
        std::cout<<"\tBaz called!\n";
        return j; 
    }
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
    
    return 0;
}
