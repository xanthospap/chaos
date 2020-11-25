#include <cassert>
#include <cstdio>
#include <iostream>
#include <random>

/// \brief Dot product.
///
/// If x, y are n-size vectors, then this algorithm computes their dot
/// product c = x^T y
///
/// \parameter[in] x Vector of size >= n.
/// \parameter[in] y Vector of size >= n.
/// \parameter[in] n Size of x, y vectors.
/// \returns         The dot product of x and y, aka x^T y
///
double dot_product(const double *__restrict__ x, const double *__restrict__ y,
                   int n) noexcept {
  double c{0e0};
  for (int i = 0; i < n; i++)
    c += x[i] * y[i];
  return c;
}

/// \brief Saxpy algorithm.
///
/// If x, y are n-size vectors and a is a real scalar, then this algorithm
/// overwrites y with ax+y.
///
void saxpy(const double *__restrict__ x, double *__restrict__ y, double a,
           int n) noexcept {
  for (int i = 0; i < n; i++)
    y[i] += a * x[i];
  return;
}

/// A is matrix R(m,n)
/// x is vector R(n)
/// y is vector R(m)
/// Compute y <- Ax + y (column version)
///
void gaxpy(const double *__restrict__ A, const double *__restrict__ x,
           double *__restrict__ y, int n /*cols*/, int m /*rows*/) noexcept {
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < m; i++) {
      // y(i) = A(i,j)x(j)+y(i)
      y[i] += A[j * m + i] * x[j];
    }
  }
  return;
}

/// A is matrix R(m,n)
/// x is vector R(m)
/// y is vector R(n)
/// Compute A <- A + xy^T (column version)
///
void gapxy(double *__restrict__ A, const double *__restrict__ x,
           const double *__restrict__ y, int n /*cols*/,
           int m /*rows*/) noexcept {
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < m; i++) {
      A[j * m + i] += y[j] * x[i];
    }
  }
}

/// A is matrix R(m,n)
/// x is vector R(m)
/// y is vector R(n)
/// Compute y <- A^Tu
///
void gatx(const double *__restrict__ A, const double *__restrict__ x,
          double *__restrict__ y, int n /*cols*/, int m /*rows*/) noexcept {
  for (int j = 0; j < n; j++) {
    y[j] = 0e0;
    for (int i = 0; i < m; i++) {
      y[j] += A[j * m + i] * x[i];
    }
  }
}
/// \brief Compute Householder vector.
///
/// Given vector x of size n, this function computes vector u (of size n), with
/// u(0) = 1 and b (scalar), such that P = I_n - buu^T is orthogonal and
/// Px = |x|e_1
///
/// \parameter[in]  x     Input vector of size >= n.
/// \parameter[in]  n     Size of vectors x and u. Only their first n elements
///                       are considered (i.e. both x and u are considered
///                       n-sized).
/// \parameter[out] u     Vector of size >= n; at output it contains the result
///                       vector u. Only its first n elements are read/written.
/// \return               beta (i.e. b).
///
/// \note A production version of this algorithm, may involve a preliminary
///       scaling (i.e. normalization) of the x vector to avoid overflow; i.e.
///       scale x to x <- x / |x|.
///
/// \ref  Matrix Computations, G.H. Colub, CF.V. Loan, 1996, pg. 210
double house(const double *__restrict__ x, int n, double *__restrict__ u) {
  // ptr to the new householder vector
  *u = 1.0e0;

  double sigma = 0e0;
  for (int i = 1; i < n; ++i) {
    sigma += x[i] * x[i];
    u[i] = x[i];
  }

  double beta = 0e0;
  if (sigma == 0e0) {
    beta = 0e0;
  } else {
    double mi = std::sqrt(x[0] * x[0] + sigma);
    if (x[0] <= 0e0) {
      u[0] = x[0] - mi;
    } else {
      u[0] = -1e0 * sigma / (x[0] + mi);
    }
    beta = 2e0 * u[0] * u[0] / (sigma + u[0] * u[0]);
  }

  double tmp = *u;
  for (int i = 0; i < n; ++i)
    u[i] /= tmp;

  return beta;
}

void householder_qr(double *A, int rows, int cols) {
  double u[rows], C[rows * cols];
  int ROWS = rows, COLS = cols;
  double b;
  double u2[rows], b2;
  for (int i = 0; i < rows; i++)
    u[i] = u2[i] = 0e0;

  for (int j = 0; j < COLS; j++) {
    b = house2(&A[j * ROWS + j], ROWS - j, u);
    b2 = house(&A[j * ROWS + j], ROWS - j, u2);
    // for (int i=0;i<ROWS-j;i++) assert (u[i] == u2[i]);
    assert(b == b2);
    for (int col = j; col < COLS; col++) {
      for (int row = j; row < ROWS; row++) {
        C[(col - j) * ROWS + row - j] = 0e0;
        for (int k = 0; k < ROWS - j; k++) {
          C[(col - j) * ROWS + row - j] -=
              u[row - j] * u[k] * A[col * ROWS + k + j];
        }
        C[(col - j) * ROWS + row - j] *= b;
        C[(col - j) * ROWS + row - j] += A[col * ROWS + row];
      }
    }
    for (int col = 0; col < COLS - j; col++) {
      for (int row = 0; row < ROWS - j; row++) {
        A[(col + j) * ROWS + (row + j)] = C[col * ROWS + row];
      }
    }
    if (j < ROWS - 1) {
      for (int row = j + 1; row < ROWS; row++) {
        A[j * ROWS + row] = u[row - j];
      }
    }
  }
  return;
}

void qrdcmp(double *a, int n, int &sign) {
  int i, j, k;
  double scale, sigma, sum, tau;
  double d[n * n], c[n * n];

  sign = 0;
  for (k = 0; k < n - 1; k++) {
    scale = 0e0;
    for (i = k; i < n; i++)
      scale = std::max(scale, std::abs(a[i * n + j]));
    if (scale == 0e0) {
      sign = 1;
      c[k] = d[k] = 0e0;
    } else {
      for (i = k; i < n; i++)
        a[i * n + k] /= scale;
      for (sum = 0.0, i = k; i < n; i++)
        sum += (a[i * n + k] * a[i * n + k]);
      sigma = std::copysign(std::sqrt(sum), a[k * n + k]);
      a[k * n + k] += sigma;
      c[k] = sigma * a[k * n + k];
      d[k] = -scale * sigma;
      for (j = k + 1; j < n; j++) {
        for (sum = 0.0, i = k; i < n; i++)
          sum += a[i * n + k] * a[i * n + j];
        tau = sum / c[k];
        for (i = k; i < n; i++)
          a[i * n + j] -= tau * a[i * n + k];
      }
    }
  }
  d[n - 1] = a[(n - 1) * n + n - 1];
  if (d[n - 1] == 0e0)
    sign = 1;
}

int main() {
  /*
  int ROWS = 4;
  int COLS = 3;
  for (int j = 0; j<COLS; j++) {
      printf("Column: %1d\n", j);
      printf("\thouse of A(%1d:%1d,%1d)\n",j, ROWS, j);
      printf("\tSubmatrix A(%1d:%1d,%1d:%1d)\n", j, ROWS, j, COLS);
      for (int col = j; col < COLS; col++) {
          for (int row = j; row < ROWS; row++) {
              printf("\n\tC(%1d,%1d) = C[%2d] = ", row-j, col-j,
  (col-j)*ROWS+row-j); for (int k=0; k<ROWS-j;k++) {
                  printf("+u(%1d)*u(%1d)*A(%1d,%1d)", row-j, k, k+j, col);
              }
              printf("\n\tA(%1d,%1d) = A[%2d]\n", row, col, col*ROWS+row);
          }
      }
  }
  */

  /*
  std::random_device rd; // obtain a random number from hardware
  std::mt19937 eng(rd()); // seed the generator
  std::uniform_real_distribution<double> distr(-25e0, 25e0); // define the range

  // Construct and randomize matrix/vectors
  const int rows = 5;
  const int cols = 3;
  double x[rows],
         y[cols],
         A[rows*cols];
  for (int i=0; i<rows; i++) {
      x[i] = distr(eng);
      for (int j=0; j<cols; j++) {
          A[i*cols+j] = distr(eng);
          y[j] = distr(eng);
      }
  }


  // print matrix/vectors
  printf("Vector x:\n");
  for (int  i=0; i<rows; i++) printf("%+15.10f ", x[i]);
  printf("\n");
  printf("Vector y:\n");
  for (int  i=0; i<cols; i++) printf("%+15.10f ", y[i]);
  printf("\n");
  printf("Matrix A:\n");
  for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++) {
          printf("%+15.10f ",A[j*rows+i]);
      }
      printf("\n");
  }

  /// A is matrix R(m,n)
  /// x is vector R(n)
  /// y is vector R(m)
  /// Compute y <- Ax + y (column version)
  printf("x <- Ay + x:\n");
  gaxpy(A, y, x, cols, rows);
  for (int  i=0; i<rows; i++) printf("%+15.10f ", x[i]);
  printf("\n");

  /// A is matrix R(m,n)
  /// x is vector R(m)
  /// y is vector R(n)
  /// Compute A <- A + xy^T (column version)
  printf("A <- A + xy^T:\n");
  gapxy(A, x, y, cols, rows);
  for (int i=0; i<rows; i++) {
      for (int j=0; j<cols; j++) {
          printf("%+15.10f ",A[j*rows+i]);
      }
      printf("\n");
  }

  /// A is matrix R(m,n)
  /// x is vector R(m)
  /// y is vector R(n)
  /// Compute y <- A^Tu
  printf("\ny <- A^Tx:\n");
  gatx(A, x, y, cols, rows);
  for (int  i=0; i<cols; i++) printf("%+15.10f ", y[i]);
  */

  int ROWS = 4, COLS = 3;
  double A[] = {1.0e0, 4.0e0, 7.0e0,  10.0e0, 2.0e0, 2.0e0,
                8.0e0, 1.0e0, 11.0e0, 6.0e0,  6.0e0, 5.0e0};
  printf("\nMatrix A:\n");
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      printf("%+15.10f ", A[j * ROWS + i]);
    }
    printf("\n");
  }

  /*
  ROWS = 3;
  COLS = 3;
  qrdcmp(A, ROWS, COLS);
  printf("\nMatrix A:\n");
  for (int i=0; i<ROWS; i++) {
      for (int j=0; j<COLS; j++) {
          printf("%+15.10f ",A[j*ROWS+i]);
      }
      printf("\n");
  }
  */

  householder_qr(A, ROWS, COLS);
  printf("\nMatrix A:\n");
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      printf("%+15.10f ", A[j * ROWS + i]);
    }
    printf("\n");
  }

  printf("\n");
  return 0;
}
