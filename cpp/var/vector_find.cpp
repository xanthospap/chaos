#include <algorithm>
#include <cstdio>
#include <iterator>
#include <vector>

struct s {
  int val;
  explicit s(int i = 2) noexcept : val(i){};
  bool is_odd() const noexcept { return val % 2; }
};

/// ------------------------------------------------------------------------
///   EXAMPLE:
///   How to use reverse_iterator to find in element in a vector
/// -------------------------------------------------------------------------
/*
int main()
{
    std::vector<int> vecofints = {2,4,2,6,8,2,5,10,10,2,4,9,2};
    std::vector<s>   vecofs;

    // print original vector and make a vector of s's
    for (auto it = cbegin(vecofints); it != cend(vecofints); ++it) {
        vecofs.emplace_back(*it);
        printf("[%2i] = %3i\n", std::distance(vecofints.cbegin(), it), *it);
    }

    // find first odd element
    auto it = std::find_if(cbegin(vecofs), cend(vecofs),
        [](const s& i){return i.is_odd();});
    printf("First odd element is %3i at index %2i\n", it->val,
        std::distance(vecofs.cbegin(), it));

    // find last odd element
    // **WARNING** we need to subtract 1 to find the values index i.e.
    // std::distance(rit, crend(vecofs))-1
    auto rit = std::find_if(crbegin(vecofs), crend(vecofs),
        [](const s& i){return i.is_odd();});
    printf("Last odd element is %3i at index %2i\n", rit->val,
        std::distance(rit, crend(vecofs))-1);

    // let's make things a bit more complicated and add a pointer to the vector
    std::vector<s>* vecptr = &vecofs;

    // find first odd element
    it = std::find_if(cbegin(*vecptr), cend(*vecptr),
        [](const s& i){return i.is_odd();});
    printf("First odd element is %3i at index %2i\n", it->val,
        std::distance(vecptr->cbegin(), it));


    printf("\n");
    return 0;
}
*/

template <typename T> class myvec {
private:
  std::vector<T> m_vec;

public:
  explicit myvec(const std::vector<T> &v) : m_vec{v} {};
  auto begin() { return m_vec.begin(); }
  auto end() { return m_vec.end(); }
};

template <typename T> class running_window {
private:
  std::size_t m_window;
  myvec<T> &m_vec;
  typename std::vector<T>::iterator m_start, m_stop, m_cur;

public:
  running_window(std::size_t w, myvec<T> &v)
      : m_window{w},
        m_vec(v), m_start{m_vec.begin()}, m_stop{m_vec.begin()}, m_cur{
                                                                     m_start} {}

  // no copy
  running_window(const running_window &) = delete;
  // no assignement
  running_window &operator=(const running_window &) = delete;

  auto begin() {
    m_start = m_cur = m_vec.begin();
    m_stop = (std::distance(m_cur, m_vec.end()) > m_window / 2)
                 ? m_cur + (m_window / 2)
                 : m_vec.end();
    return m_cur;
  }

  auto next() {
    ++m_cur;
    while (std::distance(m_start, m_cur) > (m_window / 2))
      ++m_start;
    while (std::distance(m_cur, m_stop) < (m_window / 2) &&
           m_stop != m_vec.end())
      ++m_stop;
    return m_cur;
  }

  auto end() { return m_vec.end(); }

  auto get_vec() { return (std::vector<T>{m_start, m_stop}); }

  std::size_t where_am_i() { return std::distance(m_vec.begin(), m_cur); }
};

int main() {

  std::vector<int> vecofints = {2, 4, 2, 6, 8, 2, 5, 10, 10, 2, 4, 9, 2};
  // print original vector
  for (auto it = cbegin(vecofints); it != cend(vecofints); ++it) {
    printf("[%2i] = %3i\n", std::distance(vecofints.cbegin(), it), *it);
  }

  myvec<int> mvec{vecofints};
  running_window<int> rw(4, mvec);

  for (auto it = rw.begin(); it != rw.end(); it = rw.next()) {
    std::vector<int> w = rw.get_vec();
    printf("\n\tAt index %%2i the window is: [", rw.where_am_i());
    for (auto j = w.begin(); j != w.end(); ++j) {
      printf(" %3i ", *j);
    }
    printf("]");
  }

  printf("\n");
  return 0;
}
