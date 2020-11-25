#include <iostream>

// compile time factorial array
template<typename T, int N> struct fact_table {
  T array[N];
  constexpr fact_table() : array{} {
    array[0] = array[1] = 1e0;
    for (int i=2; i<N; i++)
      array[i] = static_cast<T>(i) * array[i-1];
  }
};

// compile time Pascal-triangle
template<typename T, int N> struct pascal_triangle {
  T array[N][N];
  constexpr T element(int row, int col) noexcept {
    if (row==0) {         // first row
      if (col==0)
        return T(1e0);
      return T(0e0);
    } else if (row==1) { // second row
      if (col<2)
        return T(1e0);
      else
        return T(0e0);
    } else {
      if (col==0)
        return T(1e0);
      else if (col==row)
        return T(1e0);
      else if (col>row)
        return T(0e0);
      else
        return element(row-1,col)+element(row-1,col-1);
    }
  }
  constexpr pascal_triangle() : array{} {
    for (int i=0; i<N; i++)
      for (int j=0; j<N; j++)
        array[i][j] = element(i,j);
  }
};

int main() {
  constexpr fact_table<double, 5> fac{};
  static_assert(fac.array[0] == 1e0);
  static_assert(fac.array[1] == 1e0);
  static_assert(fac.array[2] == 2e0);
  static_assert(fac.array[3] == 6e0);
  static_assert(fac.array[4] == 24e0);
  std::cout<<"\nFactorial array ok!";
  
  constexpr pascal_triangle<double, 5> pasc{};
  static_assert(pasc.array[0][0]==1e0);
  static_assert(pasc.array[1][0]==1e0);
  static_assert(pasc.array[1][1]==1e0);
  static_assert(pasc.array[2][1]==2e0);
  for (int i=0; i<5; i++) {
    printf("\n");
    for (int j=0; j<5; j++)
      printf(" %5.1f", pasc.array[i][j]);
  }
  std::cout<<"\n";
  return 0;
}
