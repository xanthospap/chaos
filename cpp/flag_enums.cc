#include <type_traits>
#include <iostream> /* for overloading a dummy '<<' operator */

/*
 * ///////////////////////////////////////////////////////////////////////////
 *  A non-generic way to implement a flag class using strongly-typed enums.
 * ///////////////////////////////////////////////////////////////////////////
 *
/// Enumeration type to classify a satellite state vector.
class state_flag
{
public:
    /// underlying enumeration type.
    typedef int flag_integral_type;

private:
    /// The actual flag
    flag_integral_type _flag;

public:
    /// A satellite state vector can be:
    enum class flag_option : flag_integral_type
    {
        bad_or_absent = 1,
        unknown_acc   = 2,
        maneuver      = 4,
        prediction    = 8,
        no_velocity   = 16,
        no_vel_acc    = 32
    };

    /// Default constructor (nothing set)
    explicit state_flag() noexcept : _flag{0} {};
    
    /// Constructor from state
    explicit state_flag(flag_option f) noexcept
        : _flag{ static_cast<flag_integral_type>(f) }
    {}

    /// Constructor combining multiple flags
    state_flag(std::initializer_list<flag_option>&& flags)
        : _flag{0}
    {
        for (auto f : flags) _flag |= static_cast<flag_integral_type>(f);
    }

    /// Cast to (flag) underlying type
    flag_integral_type as_integral() const noexcept { return _flag; }
    
    /// Set a flag
    void set(flag_option f) noexcept
    { _flag |= static_cast<flag_integral_type>(f); }
    
    /// Clear a flag
    void clear(flag_option f) noexcept
    { _flag &= ~(static_cast<flag_integral_type>(f)); }
    
    /// Check if a flag is set
    bool check(flag_option f) const noexcept
    { return _flag & static_cast<flag_integral_type>(f); }

    /// dummy print
    friend
    std::ostream& operator<<(std::ostream& os, state_flag state)
    {
        os << state.as_integral();
        return os;
    }

}; // state_flag

int main()
{
    state_flag f1;
    
    state_flag f2 ( state_flag::flag_option::maneuver );

    state_flag f3 { state_flag::flag_option::maneuver,
                    state_flag::flag_option::no_velocity };

    std::cout << "f1 = " << f1 << "\n";
    std::cout << "f2 = " << f2 << "\n";
    std::cout << "f3 = " << f3 << "\n";

    return 0;
}
*/

/*
 * ///////////////////////////////////////////////////////////////////////////
 *  A generic way to implement the above for whatever enum class (i.e. strongly
 *  typed). This works for C++14.
 * ///////////////////////////////////////////////////////////////////////////
 */

/* the actual enums */
enum class flag_option : int
{
    bad_or_absent = 1,
    unknown_acc   = 2,
    maneuver      = 4,
    prediction    = 8,
    no_velocity   = 16,
    no_vel_acc    = 32
};

/* wrapper class around an enum to make it act like a flag class */
template<typename F> class Flag
{
public:
    using ft = typename std::underlying_type<F>::type;

private:
    ft _f;

public:
    Flag() noexcept : _f{} {}
    explicit Flag(F f) : _f{static_cast<ft>(f)} {}
    explicit Flag(std::initializer_list<F>&& fs) : _f{}
    {
        for (auto& f : fs) { _f |= static_cast<ft>(f); }
    }
    /// Set a flag
    void set(F f)         noexcept { _f |= static_cast<ft>(f); }
    /// Clear a flag
    void clear(F f)       noexcept { _f &= ~(static_cast<ft>(f)); }
    /// Check if a flag is set
    bool check(F f) const noexcept { return _f & static_cast<ft>(f); }
    /// Dummy print
    friend
    std::ostream& operator<<(std::ostream& os, const Flag f)
    {
        os << f._f;
        return os;
    }
};

/* example usage */
int main()
{
    Flag<flag_option> f1;
    Flag<flag_option> f2(flag_option::unknown_acc);
    Flag<flag_option> f3{flag_option::unknown_acc, flag_option::no_velocity};
    std::cout << "f1 = " << f1 << "\n";
    std::cout << "f2 = " << f2 << "\n";
    std::cout << "f3 = " << f3 << "\n";
    return 0;
}
