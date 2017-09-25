#include <iostream>
#include <random>
#include <cstdio>

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
double
dot_product(const double *__restrict__ x, const double *__restrict__ y, int n)
noexcept
{
    double c {0e0};
    for (int i=0; i<n; i++) c += x[i]*y[i];
    return c;
}

/// \brief Saxpy algorithm.
///
/// If x, y are n-size vectors and a is a real scalar, then this algorithm
/// overwrites y with ax+y.
///
void
saxpy(const double *__restrict__ x, double *__restrict__ y, double a,
    int n)
noexcept
{
    for (int i=0; i<n; i++) y[i] += a*x[i];
    return;
}

/// A is matrix R(m,n)
/// x is vector R(n)
/// y is vector R(m)
/// Compute y <- Ax + y (column version)
///
void
gaxpy(const double *__restrict__ A,
      const double *__restrict__ x,
      double       *__restrict__ y,
      int n /*cols*/,
      int m /*rows*/)
noexcept
{
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            // y(i) = A(i,j)x(j)+y(i)
            y[i] += A[j*m+i]*x[j];
        }
    }
    return;
}


/// A is matrix R(m,n)
/// x is vector R(m)
/// y is vector R(n)
/// Compute A <- A + xy^T (column version)
///
void
gapxy(      double *__restrict__ A,
      const double *__restrict__ x,
      const double *__restrict__ y,
      int n /*cols*/,
      int m /*rows*/)
noexcept
{
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            A[j*m+i] += y[j]*x[i];
        }
    }
}

/// A is matrix R(m,n)
/// x is vector R(m)
/// y is vector R(n)
/// Compute y <- A^Tu
///
void
gatx( const double *__restrict__ A,
      const double *__restrict__ x,
            double *__restrict__ y,
      int n /*cols*/,
      int m /*rows*/)
noexcept
{
    for (int j = 0; j < n; j++) {
        y[j] = 0e0;
        for (int i = 0; i < m; i++) {
            y[j] += A[j*m+i]*x[i];
        }
    }
}

void
pa_update(double *__restrict__ A, const double *__restrict__ u, double b, int rows, int cols)
{
    int m {rows},
        n {cols};

    // w <- A^Tu
    double w[cols];
    for (int j = 0; j < n; j++) {
         w[j] = 0e0;
         for (int i = 0; i < m; i++) {
             w[j] += A[j*m+i]*u[i];
         }
    }

    // A <- A-uw^T
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            A[j*m+i] += w[j]*u[i];
        }
    }
     
    
}

void
householder_qr()
{
    // for j = 1:n i.e. for every column
    for (col = 0;  col < n; col++) {
        // u, b = house( A(j:m,j) )
        // j-th column of A, from row j to end:
        // j-th column of A is:        A[j*m]
        // j-th column at j-th row is: A[j*m+j]
        // A(j:m, j) is a vector with rows = m-j
        b = house(A[j*m+j], u, j);
        // A(j:m, j:n) is a submatrix;
        // Number of rows:    m-j
        // Number of columns: n-j

    
}

int main()
{
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

    printf("\n");
    return 0;
}
