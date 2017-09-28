#include <cmath>
#ifdef DEBUG
#include <iostream>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#endif

/// \brief This file contains algorithms connected to QR factorization.
///
/// \notes
///     - All matrix/vector indexes start from 0.
///     - All matrix/vector are stored column-wise
///

/// \brief Compute Householder vector.
///
/// Given vector x of size n, this function computes vector u (of size n), with
/// u(0) = 1 and b (scalar), such that P = I_n - buu^T is orthogonal and
/// Px = |x|e_1
///
/// \parameter[in]  x     Input vector of size >= n.
/// \parameter[in]  n     Size of vectors x and u. Only their first n elements
///                       are considered (i.e. both x and u are considered n-sized).
/// \parameter[out] u     Vector of size >= n; at output it contains the result
///                       vector u. Only its first n elements are read/written.
/// \return               beta (i.e. b).
///
/// \note A production version of this algorithm, may involve a preliminary
///       scaling (i.e. normalization) of the x vector to avoid overflow; i.e.
///       scale x to x <- x / |x|.
///
/// \ref  Matrix Computations, G.H. Colub, CF.V. Loan, 1996, pg. 210
double
house(const double *__restrict__ x, int n, double *__restrict__ u)
noexcept
{
    u[0] = 1.0e0;
    double sigma {0e0},
           beta  {0e0};
    
    for (int i = 1; i < n; ++i) {
        sigma += x[i] * x[i];
        u[i]   = x[i];
    }

    if ( sigma == 0e0 ) {
        beta = 0e0;
    } else {
        double mi = std::sqrt(x[0]*x[0]+sigma);
        if ( x[0]<= 0e0 ) {
            u[0] = x[0] - mi;
        } else {
            u[0] = -1e0 * sigma / (x[0] + mi);
        }
        beta = 2e0*u[0]*u[0] / (sigma+u[0]*u[0]);
    }

    double tmp {u[0]};
    for (int i = 0; i < n; ++i) u[i] /= tmp;

    return beta;
}

void
householder_qr(double *__restrict__ A, int rows, int cols)
{
    double u[rows],
           C[rows*cols];
    int    ROWS = rows,
           COLS = cols;
    double b;

    for (int j = 0;  j < COLS; j++) {
        b = house(&A[j*ROWS+j], ROWS-j, u);
        for (int col = j; col < COLS; col++) {
            for (int row = j; row < ROWS; row++) {
                C[(col-j)*ROWS+row-j] = 0e0;
                for (int k = 0; k < ROWS-j; k++) {
                    C[(col-j)*ROWS+row-j] -= u[row-j]*u[k]*A[col*ROWS+k+j];
               }
                C[(col-j)*ROWS+row-j] *= b;
                C[(col-j)*ROWS+row-j] += A[col*ROWS+row];
            }
        }
        for (int col = 0; col < COLS-j; col++) {
            for (int row = 0; row < ROWS-j; row++) {
                A[(col+j)*ROWS+(row+j)] = C[col*ROWS+row];
            }
        }
        if (j < ROWS-1) {
            for (int row = j+1; row < ROWS; row++) {
                A[j*ROWS+row] = u[row-j];
            }
        }
    }
    return;
}

void qrdcmp(double *__restrict__ a, int n, double *__restrict__ d, int &sign)
{
    int i,j,k;
    double scale, sigma, sum, tau;
    double c[n*n];

    sign = 0;
    for (k=0;k<n-1;k++) {
        scale = 0e0;
        for (i=k;i<n;i++) scale=std::max(scale,std::abs(a[i*n+k]));
        if (scale == 0e0) {
            sign = 1;
            c[k] = d[k] = 0e0;
        } else {
            for (i=k;i<n;i++) a[i*n+k] /= scale;
            for (sum=0.0,i=k;i<n;i++) sum += (a[i*n+k]*a[i*n+k]);
            sigma = std::copysign(std::sqrt(sum),a[k*n+k]);
            a[k*n+k] += sigma;
            c[k]=sigma*a[k*n+k];
            d[k] = -scale*sigma;
            for (j=k+1;j<n;j++) {
                for (sum=0.0,i=k;i<n;i++) sum += a[i*n+k]*a[i*n+j];
                tau=sum/c[k];
                for (i=k;i<n;i++) a[i*n+j] -= tau*a[i*n+k];
            }
        }
    }
    d[n-1] = a[(n-1)*n+n-1];
    if (d[n-1] == 0e0) sign = 1;
}


using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using namespace std::literals::chrono_literals;

int main()
{
    
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_real_distribution<double> distr(-125e0, 125e0); // define the range
    
    
    int ROWS = 4, COLS = 3;
    double A[] = {1.0e0,  4.0e0,  7.0e0,
                 10.0e0,  2.0e0,  2.0e0,
                  8.0e0,  1.0e0, 11.0e0,
                  6.0e0,  6.0e0,  5.0e0};
    printf("\nMatrix A:\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",A[j*ROWS+i]);
        }
        printf("\n");
    }

    householder_qr(A, ROWS, COLS);
    printf("\nMatrix A:\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",A[j*ROWS+i]);
        }
        printf("\n");
    }

    ROWS=COLS=3;
    double B_rw[] = {12e0,-51e0,  4e0,
                      6e0,167e0,-68e0,
                     -4e0, 24e0,-41e0 };
    double B_cw[] = { 12e0,   6e0,  -4e0,
                     -51e0, 167e0,  24e0,
                       4e0, -68e0, -41e0}; 
    int sign;
    double d[ROWS];
    qrdcmp(B_rw, 3, d, sign);
    printf("\nMatrix B:\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",B_rw[i*COLS+j]);
        }
        printf("\n");
    }
    
    householder_qr(B_cw, ROWS, COLS);
    printf("\nMatrix B:\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",B_cw[j*ROWS+i]);
        }
        printf("\n");
    }

    time_point<Clock> start,
                      end;
    milliseconds      diff;
    for (int size = 10; size < 1000; size += 200) {
        double MATRIX[size*size];
        ROWS = COLS = size;
        for (int i=0; i<size*size; i++) MATRIX[i] = distr(eng);

        start = Clock::now();
        householder_qr(MATRIX, ROWS, COLS);
        end = Clock::now();
        diff = duration_cast<milliseconds>(end - start);
        std::cout << size << " " << diff.count() << "ms\n";
        
        /*
        start = Clock::now();
        try {
            double D[ROWS];
            qrdcmp(MATRIX, ROWS, D, sign);
        } catch (std::exception& e) {
            std::cerr<<"\n"<<e.what()<<"\n";
        }
        end = Clock::now();
        diff = duration_cast<milliseconds>(end - start);
        std::cout << diff.count() << "ms\n";
        */
    }

    printf("\n");
    return 0;
}
