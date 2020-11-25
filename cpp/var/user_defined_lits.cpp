using ulli = unsigned long long int;

class A {
public:
  explicit constexpr A(long i = 0L) noexcept : mi{i} {};
  constexpr bool operator==(A a) const noexcept { return mi == a.mi; }

private:
  long mi;
};

constexpr A operator"" _a(ulli i) noexcept { return A{(long)i}; }

int main() {
  constexpr A a1{1L};
  constexpr A a2 = 1_a;
  static_assert(a1 == a2, "");
  return 0;
}
