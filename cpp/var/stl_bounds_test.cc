#include <iostream>
#include <vector>
#include <algorithm>

struct Dummy
{
  int a;
  float b, c;
};

struct Dummy_wOps
{
  int a;
  float b, c;
  bool 
  operator<(int r) const noexcept {return this->a<r;} /* comp(Dummy_wOps, int) for lower_bound */
};
bool /* comp(int, Dummy_wOps) for upper_bound */
operator<(int l, Dummy_wOps r) noexcept {return l<r.a;}

int main()
{
  const std::vector<Dummy> vec ={ {0,1,2}, {1,2,3}, {2,3,4}, {3,4,5}, {4,5,6} };
  std::cout<<"\nInitial Vector:";
  for (auto i : vec) std::cout<<"\n"<<i.a<<" ("<<i.b<<", "<<i.c<<")";

  auto it = std::lower_bound(vec.begin(), vec.end(), 2,
  // According to the reference, the implementation uses a compare function
  // as:
  // if (comp(*it, value)) {
  // see https://en.cppreference.com/w/cpp/algorithm/lower_bound
    [](const Dummy& d, int b){return d.a<b;});
  std::cout<<"\nLower bound for 2 sets iterator to: "
           <<"\n"<<it->a<<" ("<<it->b<<", "<<it->c<<")";
  
  it = std::upper_bound(vec.begin(), vec.end(), 2,
  // According to the reference, the implementation uses a compare function
  // as:
  // if (!comp(value, *it)) {
  // see https://en.cppreference.com/w/cpp/algorithm/upper_bound
    [](int b, const Dummy& d){return b<d.a;});
  std::cout<<"\nUpper bound for 2 sets iterator to: "
           <<"\n"<<it->a<<" ("<<it->b<<", "<<it->c<<")";

  const std::vector<Dummy_wOps> vec2 = { {0,1,2}, {1,2,3}, {2,3,4}, {3,4,5}, {4,5,6} };
  std::cout<<"\nInitial Vector:";
  for (auto i : vec2) std::cout<<"\n"<<i.a<<" ("<<i.b<<", "<<i.c<<")";

  auto it2 = std::lower_bound(vec2.begin(), vec2.end(), 2);
  std::cout<<"\nLower bound for 2 sets iterator to: "
           <<"\n"<<it2->a<<" ("<<it2->b<<", "<<it2->c<<")";
  
  it2 = std::upper_bound(vec2.begin(), vec2.end(), 2);
  std::cout<<"\nUpper bound for 2 sets iterator to: "
           <<"\n"<<it2->a<<" ("<<it2->b<<", "<<it2->c<<")";
  std::cout<<"\n";
  return 0;
}
