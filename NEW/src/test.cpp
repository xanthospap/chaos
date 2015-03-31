#include <iostream>

template<char C>
class Attribute {
public:
    constexpr Attribute(){};
    constexpr char attribute () {return C;}
};

enum class ENUMR : unsigned char {
    SYSTEM1,
    SYSTEM2,
    UNKNOWN
};

template<ENUMR S>
class System
{};

template<>
class System<ENUMR::SYSTEM1>
{
private:
    static constexpr char identifier { 'G' };
};

template<ENUMR Sys,short int Freq,char C>
class Obs {
public:
    Obs():
        m_system{},
        m_frequency{ Freq },
        m_attribute{}
    {};
private:
    System<Sys>  m_system;
    short int    m_frequency;
    Attribute<C> m_attribute;
};

template<ENUMR Sys,short int Freq,class Attribute>
class Obs3 {
public:
    Obs3():
        m_system{},
        m_frequency{ Freq },
        m_attribute{Attribute{}}
    {};
    constexpr char attribute () const 
    {
        return m_attribute.attribute();
    }
private:
    System<Sys>  m_system;
    short int    m_frequency;
    Attribute    m_attribute;
};

int main ()
{

    System<ENUMR::SYSTEM1> s1;

    System<ENUMR::UNKNOWN> s2;

    System<ENUMR::SYSTEM1> s3 (System<ENUMR::SYSTEM1>);

    Attribute<'M'> atr;

    Obs<ENUMR::SYSTEM1, 1, 'c'> obs;

    Obs3<ENUMR::SYSTEM1, 1, Attribute<'X'> > obs3;
    std::cout << "\nAttribute -> " << obs3.attribute ();

    const char c { 'L' }; // removing the const, produces an error
    Obs3<ENUMR::SYSTEM1, 1, Attribute<c> > obs4;

    std::cout <<"\n";
    return 0;
}
