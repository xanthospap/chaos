#include <iostream>
#include <type_traits>

class argument
{
public:
    explicit
    argument(char s, std::string l, std::string v) noexcept
    : _short_opt(s),
      _long_opt(l),
      _val(v)
    {}

    template<typename T,
             typename std::enable_if<std::is_floating_point<T>::value>::type = 0
             >
    T as_T()
    {
        return static_cast<T>(std::stod(_val));
    }

    template<typename T,
             typename std::enable_if<std::is_integral<T>::value>::type = 0
             >
    T as_T()
    {
        return static_cast<T>(std::stol(_val));
    }
    /*
    template<typename T,
             typename = std::enable_if_t<!std::is_floating_point<T>::value && !std::is_integral<T>::value>
             >
    T as_T()
    {
        return static_cast<T>(_val);
    }*/

private:
    char        _short_opt;
    std::string _long_opt;
    std::string _val;
};

/*
class argparser
{
public:
    explicit
    argparser() noexcept {};

    void
    add_argument(char s, std::string l="", std::string v="") noexcept
    {
        _args.emplace_back(s, l, v);
    }

    
private:
    std::vector<argument> _args;
};
*/

int main(/*argc, char* argv[]*/)
{
    argument a1 ('a', "alpha", "string");
    argument a2 ('b', "beta",  "15");
    argument a3 ('c', "gamma", "3.14753");

    // std::cout<<"\n a1 as string: "<<a1.as_T<std::string>();
    std::cout<<"\n a2 as int   : "<<a2.as_T<int>();
    std::cout<<"\n a3 as float : "<<a3.as_T<float>();

    std::cout<<"\n";
    return 0;
}
