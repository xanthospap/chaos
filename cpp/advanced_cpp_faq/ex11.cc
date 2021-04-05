#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

/* (1) */
template <typename InputIt>
using diff_type = typename std::iterator_traits<InputIt>::difference_type;

template <typename T, typename InputIt, typename A = std::allocator<T>>
typename std::vector<T, A>::iterator
myinsert(std::vector<T, A> &vec,
         typename std::vector<T, A>::const_iterator position, InputIt first,
         InputIt last) {
  /*
  Instead of using the following lines, define diff_type in (1)
  typename std::iterator_traits<InputIt>::difference_type position_offset =
      position - vec.begin();
  typename std::iterator_traits<InputIt>::difference_type end_offset =
      vec.end() - vec.begin(); // aka original size
  */
  diff_type<InputIt> position_offset = position - vec.begin();
  diff_type<InputIt> end_offset = vec.end() - vec.begin(); // aka original size

  // push back all elements to be inserted
  while (first != last) {
    vec.push_back(*first++);
  }

  // now rotate the new elements to position
  return std::rotate(vec.begin() + position_offset, vec.begin() + end_offset,
                     vec.end());
}

int main() {
  // The original sequence
  std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto vcp = v;

  // The sequence to be inserted into v at the position v.begin() + 4
  std::vector<int> v2{11, 12, 13, 14};

  // First,  insert  all  the elements of v2 at the back of v
  v.insert(v.end(), v2.begin(), v2.end());

  // Finally, rotate appropriately
  std::rotate(v.begin() + 4, v.begin() + 9, v.end());

  std::vector<int> vtrue{1, 2, 3, 4, 11, 12, 13, 14, 5, 6, 7, 8, 9};
  assert(v == vtrue);

  // try out our new algorithm
  myinsert(vcp, vcp.begin() + 4, v2.begin(), v2.end());
  assert(vcp == vtrue);

  std::cout << "\nAll ok!";
  return 0;
}
