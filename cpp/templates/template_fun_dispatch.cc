#include <iostream>
#include <type_traits>

// ------------------------------------------------------------------------- //
// Dispatch on some function using templates. In this case i could do
// something like foo<T>{...} and directly specialize the cases BUT this is
// not the case of foo() is a member function. Anyway, try to do it in most
// ways possible
// A more -real-world example would be:
// class bar {
// public:
// ...
//     template<typename T>
//         foo(/*...*/) { /* dispatch on foo_impl using T (or an enum) */ }
// private:
// ...
//     foo_impl(/*...*/, true_type ) { /* overload 1 */ }
//     foo_impl(/*...*/, false_type) { /* overload 2 */ }
// };
// int main() {
//     bar b;
//     b</*...*/>(/*...*/);
//  };
// ------------------------------------------------------------------------- //

/*
 *  A. This works fine
 *  ------------------
struct normal_store  { static constexpr std::true_type tid() {return
std::true_type{};} }; struct reverse_store { static constexpr std::false_type
tid() {return std::false_type{};} }; void foo_impl(int k, std::true_type)  {
std::cout << "\nimpl 1\n"; } void foo_impl(int k, std::false_type) { std::cout
<< "\nimpl 2\n"; } template<typename T> void foo(int k)
{
    return foo_impl( k, T::tid() );
}
*/

/*
 * B. This works fine
 * ------------------
struct normal_store  { using t = std::true_type; };
struct reverse_store { using t = std::false_type; };
void foo_impl(int k, std::true_type)  { std::cout << "\nimpl 1\n"; }
void foo_impl(int k, std::false_type) { std::cout << "\nimpl 2\n"; }
template<typename T>
    void foo(int k)
{
    using t = typename T::t;
    return foo_impl( k, t{} );
}
*/

/*
 * C. This does work (at last)!
 * ----------------------------
 */
enum class store { normal_store, reverse_store };
void foo_impl(int k, std::true_type) { std::cout << "\nimpl 1\n"; }
void foo_impl(int k, std::false_type) { std::cout << "\nimpl 2\n"; }
template <store E> void foo(int k) {
  typedef std::integral_constant<store, store::normal_store> tt1;
  typedef std::integral_constant<store, E> tt3;
  using t = std::is_same<tt1, tt3>;
  return foo_impl(k, t{});
}

int main() {
  foo<store::normal_store>(1);
  foo<store::reverse_store>(1);
  return 0;
}
