#include <iostream>

template<char C>
class Attribute {};

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

template<ENUMR Sys,short int Freq,
    template <typename> class Attribute, typename C>
class Obs {
public:
    Obs():
        m_system{},
        m_frequency{ Freq }//,
        //m_a {a}
    {};
private:
    System<Sys> m_system;
    short int   m_frequency;
    //A           m_a;
};

int main ()
{

    System<ENUMR::SYSTEM1> s1;

    System<ENUMR::UNKNOWN> s2;

    System<ENUMR::SYSTEM1> s3 (System<ENUMR::SYSTEM1>);

    // Attribute<'M'> atr;

    Obs<ENUMR::SYSTEM1, 1, Attribute,char> obs;

    std::cout <<"\n";
    return 0;
}
