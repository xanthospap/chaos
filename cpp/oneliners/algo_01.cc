#include <cassert>
#include <algorithm>
#include <string>

/* python one-liner
 * is_anagram = lambda x1, x2: sorted(x1) == sorted(x2)
 */

// std::sort is constexpr after c++20
bool is_anagram(const std::string& s1, const std::string& s2) noexcept {
  return std::sort(s1.cbegin(), s1.cend()) == std::sort(s2.cbegin(), s2.cend());
}

int main() {
  assert(is_anagram("elvis", "lives") == true);
  assert(is_anagram("elvise", "livees") == true);
  assert(is_anagram("elvis", "dead") == true);
  return 0;
}
