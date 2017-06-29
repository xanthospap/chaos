#include <iostream>
#include <type_traits>
#include <vector>

template<int NumArgs=1>
    class argument
{
public:
    explicit
    argument(char s, std::string l, std::string v) noexcept
    : _short_opt(s),
      _long_opt(l),
      _val(v)
    {}

    /// Return value as any floating point value type (only available when
    /// T is floating point value).
    template<typename T>
        typename std::enable_if<std::is_floating_point<T>::value, T>::type
        as_T()
    {
        return static_cast<T>(std::stod(_val));
    }

    /// Return value as any signed integer type (only available when T is
    /// signed integer).
    template<typename T>
        typename std::enable_if<std::numeric_limits<T>::is_integer
                             && std::is_signed<T>::value,
                              T>::type
        as_T()
    {
        return static_cast<T>(std::stol(_val));
    }
    
    /// Return value as any unsigned integer type (only available when T is
    /// unsigned integer).
    template<typename T>
        typename std::enable_if< std::numeric_limits<T>::is_integer
                             && (!std::is_signed<T>::value),
                              T>::type
        as_T()
    {
        return static_cast<T>(std::stoul(_val));
    }
    
    /// Return a value that is not integral and not floating point
    template<typename T>
        typename std::enable_if<(!std::is_floating_point<T>::value
                              && !std::is_integral<T>::value),
                               T>::type
        as_T()
    {
        return static_cast<T>(_val);
    }

private:
    char        _short_opt;
    std::string _long_opt;
    std::string _val;

    std::vector<std::string>
    split_args(char delim=' ') const
    {
        std::vector<std::string> v;
        typename std::string::size_type start = 0;
        auto pos = _val.find_first_of(delim, start);
        while (pos != std::string::npos) {
            if (pos != start) // ignore empty tokens
                v.emplace_back(_val, start, pos - start);
            start = pos + 1;
            pos   = _val.find_first_of(delim, start);
        }
        if (start < _val.length()) // ignore trailing delimiter
            v.emplace_back(_val, start, _val.length() - start); // add what's left of the string
        return v;
    }
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

    std::cout<<"\n a1 as string: "<<a1.as_T<std::string>();
    std::cout<<"\n a2 as int   : "<<a2.as_T<int>();
    std::cout<<"\n a3 as float : "<<a3.as_T<float>();

    std::cout<<"\n";
    return 0;
}
