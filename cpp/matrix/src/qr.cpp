#include <cmath>
#ifdef DEBUG
#include <iostream>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#endif

/// \brief Back substitution, column version.
///
/// If \f$U\in {\Re}^{n\times n}\f$ is upper triangular and \f$b\in{\Re}^n \f$,
/// then this algorithm overwrittes b with the solution to \f$Ux=b\f$. U is
/// assumed to be nonsingular.
///
/// \param[in]     u  Upper-triangular matrix, in column-majorm storage; its size
///                   is nxn.
/// \param[in,out] b  At input the right-hand side vector of the equation Ux=b.
///                   At output, it is overwritten by the solution vector x.
/// \param[in]     n  Size of the matrix U and vectors b and x.
///
/// Reference, Matrix Computations, G.H. Colub, CF.V. Loan, 1996, pg. 90
void
back_substitution(const double *__restrict__ u, double *__restrict__ b, int n)
noexcept
{
    int i,j;
    for (j = n-1; j > 0; j--) {
        b[j] /= u[j*n+j];
        for (i = 0; i < j-1; i++) b[j] -= b[j]*u[j*n+i];
    }
    b[0] /= u[0];
    return;
}


/// \brief Compute Householder vector.
///
/// Given \f$x\in{\Re}^{n}\f$, this function computes vector \f$u\in{\Re}^{n}\f$,
/// with \f$u(0)=1\f$ and \f$\beta \in \Re\f$ (scalar), such that 
/// \f$P = I_n - \beta uu^T\f$ is orthogonal and \f$Px = \|x\|_2 e_1\f$.
///
/// \param[in]  x     Input vector of size >= n (aka \f$x\in{\Re}^{n}\f$)
/// \param[in]  n     Size of vectors x and u. Only their first n elements
///                   are considered (both x and u can have sizes larger than
///                   n, but not smaller).
/// \param[out] u     Vector of size >= n; at output it contains the result
///                   Householder vector u. Only its first n elements are 
///                   read/written (aka \f$u\in{\Re}^{n}\f$).
/// \return           beta (aka \f$\beta\in\Re\f$).
///
/// \note A production version of this algorithm, may involve a preliminary
///       scaling (i.e. normalization) of the x vector to avoid overflow; i.e.
///       scale x to x <- x / |x|.
///
/// Reference: Matrix Computations, G.H. Colub, CF.V. Loan, 1996, pg. 210
double
householder_vec(const double *__restrict__ x, int n, double *__restrict__ u)
noexcept
{
    u[0] = 1.0e0;
    double sigma {0e0},
           beta  {0e0};
    
    for (int i = 1; i < n; ++i) {
        sigma += x[i]*x[i];
        u[i]   = x[i];
    }

    if ( sigma == 0e0 ) {
        beta = 0e0;
    } else {
        double mi = std::sqrt(x[0]*x[0]+sigma);
        if ( x[0]<= 0e0 ) {
            u[0] = x[0]-mi;
        } else {
            u[0] = -sigma/(x[0]+mi);
        }
        beta = 2e0*u[0]*u[0]/(sigma+u[0]*u[0]);
    }

    double tmp {u[0]};
    for (int i = 0; i < n; ++i) u[i] /= tmp;

    return beta;
}

/// \brief Compute Householder QR decomposition.
///
/// Given \f$A\in{\Re}^{m\times n}\f$ with \$fm\geq n\f$, the following
/// algorithm finds Householder matrices \f$H_1, H_2 ..., H_n\f$ such that if
/// \f$Q= H_1 H_2 ... H_n \f$, then \f$Q^T A = R \f$ is upper triangular. The
/// upper triangular part of A is overwritten by the upper triangular part of
/// R and components j+1:m of the jth Householder vector are stored in
/// \f$A(j+1:m, j), j<m\f$.
/// Matrix A must be stored column-wise.
/// 
/// \param[in]  a    The matrix \f$A\in{\Re}^{m\times n}\f$ (column-wise).
/// \param[in]  b    A vector of size n; at output, it contains the \f$\beta\f$
///                  coefficients for each of the n-1 Householder vectors.
/// \param[in]  m    Number of rows of a matrix A.
/// \param[in]  n    Number of columns of matrix A.
/// \param[out] sign Unused.
///
/// ref  Matrix Computations, G.H. Colub, CF.V. Loan, 1996, pg. 224
void
householder_qr_alpha(double *__restrict__ a, double *__restrict__ b, int m, int n,
    int &sign)
noexcept
{
    double sum,mi,*u;
    int    row,col,j;

    try {
        u = new double[m];
    } catch (std::bad_alloc&) {
        sign = 1;
        return;
    }

    for (col = 0; col < n; col++) {
        //  compute u vector (i.e. Householder vector) based on the input
        //+ vector A(col:m, col).
        for (sum = 0e0, row = col+1; row < m; row++) sum += a[col*m+row]*a[col*m+row];
        mi      = std::sqrt(a[col*m+col]*a[col*m+col]+sum);
        u[0]    = (a[col*m+col]>=0e0)
                ? (-sum/(a[col*m+col]+mi))
                : (a[col*m+col]-mi);
        b[col]  = 2e0*(u[0]*u[0])/(sum+u[0]*u[0]);
        for (row = col+1; row < m; row++) u[row-col] = a[col*m+row]/u[0];
        u[0] /= u[0];

        // Compute A(col, col)
        for (sum = 0e0, row = col; row < m; row++) sum += a[col*m+row]*u[row-col];
        a[col*m+col] -= sum*b[col];
        
        // Assign householder vector to A(col+1:m, col)
        for (row = col+1; row < m; row++) a[col*m+row] = u[row-col];
        
        // Compute A(col+1:m, col+1:n) = (I-buu^T)A(col+1:m, col+1:n)
        for (j = col+1; j < n; j++) {
            for (sum = 0e0, row = col; row < m; row++) sum += a[j*m+row]*u[row-col];
            sum *= b[col];
            for (row = col; row < m; row++) a[j*m+row] -= sum*u[row-col];
        }
    }
}

void
householder_qr(double *__restrict__ a, double *__restrict__ b, int m, int n,
    int &sign)
noexcept
{
    double sum,sigma;
    int    row,col,j;
    double d[n];

    for (col = 0; col < n; col++) {
        //  compute u vector (i.e. Householder vector) based on the input
        //+ vector A(col:m, col).
        for (sum = 0e0, row = col+1; row < m; row++) sum += a[col*m+row]*a[col*m+row];
        sigma = std::copysign(std::sqrt(sum), a[col*m+col]);
        a[col*m+col] += sigma;
        b[col] = sigma*a[col*m+col];
        d[col] = -sigma;

        /* Compute A(col, col)
        for (sum = 0e0, row = col; row < m; row++) sum += a[col*m+row]*u[row-col];
        a[col*m+col] -= sum*b[col]; */
        
        /* Assign householder vector to A(col+1:m, col)
        for (row = col+1; row < m; row++) a[col*m+row] = u[row-col]; */
        
        // Compute A(col+1:m, col+1:n) = (I-buu^T)A(col+1:m, col+1:n)
        for (j = col+1; j < n; j++) {
            for (sum = 0e0, row = col; row < m; row++) sum += a[j*m+row]*a[col*m+row];
            sum /= b[col];
            for (row = col; row < m; row++) a[j*m+row] -= sum*a[col*m+row];
        }
    }
    return;
}

/// \brief Householder LS solution.
///
/// If \f$A \in {\Re}^{m \times n}\f$ has full comumn rank and 
/// \f$b \in {\Re}^m \f$, then this algorithm computes a vector \f$x_{LS}\f$
/// such that \f$ \| A x_{LS} - b \| \f$ is minimum.
void
ls_qrsolve(double *__restrict__ a, double *__restrict__ b, int m, int n)
{
    double beta[m], sum;
    int i, j, sign;

    // Overwrite a with its QR factorization
    householder_qr(a, beta, m, n, sign);

    // Compute b <- (Q^T)*b
    for (j = 0; j < n; j++) {
        for (sum = b[j], i = j+1; i < m; i++) sum += a[j*m+i]*b[i];
        for (i = j; i < m; i++) b[i] -= beta[j]*a[j*m+i];
    }

    // Solve R(1:n,1:n) * x = n(1:n)
    for (j = n-1; j > 0; j--) {
        b[j] /= a[j*m+j];
        for (i = 0; i < j-1; i++) b[j] -= b[j]*a[j*m+i];
    }
    b[0] /= a[0];

    return;
}

/// \brief Compute Householder QR decomposition for a square matrix.
///
/// Constructs the QR decomposition of \f$A\in {\Re}^{n \times n}\f$. The 
/// upper triangular matrix R is returned in the upper triangle of A, except
/// for the diagonal elements of R which are returned in d[0..n-1]. The 
/// orthogonal matrix Q is represented as a product of n−1 Householder matrices
/// \f$Q_1 ... Q_{n−1}\f$, where \f$Q_j = 1 − u_j {u_j}^T / c_j \f$ . The ith
/// component of \f$u_j\f$ is zero for i=1, . . . , j−1. sing returns as
/// true (1) if singularity is encountered during the decomposition, but the
/// decomposition is still completed in this case; otherwise it returns
/// false (0).
/// Matrix A must be stored row-wise.
///
/// \param[in]  a    The matrix \f$A\in{\Re}^{n\times n}\f$ (row-wise).
/// \param[out] c    A vector of size n; at output, it contains the \f$c\f$
///                  coefficients for each of the n-1 Householder vectors.
/// \param[out] d    A vector of size n; at output, it contains the diagonal
///                  elements of R.
/// \param[in]  n    Number of columnsa and rows of matrix A.
/// \param[out] sign True if singularity encountered; otherwise 0.
///
/// ref Numerical Recipies in C, A.S. Teukolski et al, pg. 99
void 
qrdcmp(double *__restrict__ a, double *__restrict__ c, double *__restrict__ d,
    int n, int &sign)
noexcept
{
    int    i, j, k;
    double scale, sigma, sum, tau;

    sign = 0;
    for (k = 0; k < n-1; k++) {
        scale = 0e0;
        for (i = k; i < n;i++) scale=std::max(scale,std::abs(a[i*n+k]));
        if (scale == 0e0) {
            sign = 1;
            c[k] = d[k] = 0e0;
        } else {
            for (i = k; i < n; i++) a[i*n+k] /= scale;
            for (sum = 0.0, i = k; i < n; i++) sum += (a[i*n+k]*a[i*n+k]);
            sigma = std::copysign(std::sqrt(sum), a[k*n+k]);
            a[k*n+k] += sigma;
            c[k] = sigma*a[k*n+k];
            d[k] = -scale*sigma;
            for (j = k+1; j < n; j++) {
                for (sum = 0.0, i = k; i < n; i++) sum += a[i*n+k]*a[i*n+j];
                tau = sum/c[k];
                for (i = k; i < n; i++) a[i*n+j] -= tau*a[i*n+k];
            }
        }
    }
    d[n-1] = a[(n-1)*n+n-1];
    if (d[n-1] == 0e0) sign = 1;
#ifdef DEBUG
/*
    printf("\n\tDiagonal part of R is:");
    for (int l=0; l<n; l++) printf("%5.1f ", d[l]);
    printf("\n\tHouseholder vectors:");
    for (int l=0; l<n-1; l++) {
        printf("\n\t");
        for (int m=l+1; m<n; m++) {
            printf("%5.1f ", a[m*n+l]*c[l]);
        }
    }
*/
#endif
    return;
}
