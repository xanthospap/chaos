#include <cmath>
#ifdef DEBUG
#include <iostream>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#endif

/// @brief Back substitution, column version.
///
/// If \f$U\in {\Re}^{n\times n}\f$ is upper triangular and \f$b\in{\Re}^n \f$,
/// then this algorithm overwrittes b with the solution to \f$Ux=b\f$. U is
/// assumed to be nonsingular.
///
/// @param[in]     u  Upper-triangular matrix, in column-majorm storage; its size
///                   is nxn.
/// @param[in,out] b  At input the right-hand side vector of the equation Ux=b.
///                   At output, it is overwritten by the solution vector x.
/// @param[in]     n  Size of the matrix U and vectors b and x.
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


/// @brief Compute Householder vector.
///
/// Given \f$x\in{\Re}^{n}\f$, this function computes vector \f$u\in{\Re}^{n}\f$,
/// with \f$u(0)=1\f$ and \f$\beta \in \Re\f$ (scalar), such that 
/// \f$P = I_n - \beta uu^T\f$ is orthogonal and \f$Px = \|x\|_2 e_1\f$.
///
/// @param[in]  x     Input vector of size >= n (aka \f$x\in{\Re}^{n}\f$)
/// @param[in]  n     Size of vectors x and u. Only their first n elements
///                   are considered (both x and u can have sizes larger than
///                   n, but not smaller).
/// @param[out] u     Vector of size >= n; at output it contains the result
///                   Householder vector u. Only its first n elements are 
///                   read/written (aka \f$u\in{\Re}^{n}\f$).
/// @return           beta (aka \f$\beta\in\Re\f$).
///
/// @note A production version of this algorithm, may involve a preliminary
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

/// @brief Compute Householder QR decomposition.
///
/// Given \f$A\in{\Re}^{m\times n}\f$ with \f$m\geq n\f$, the following
/// algorithm finds Householder matrices \f$H_1, H_2 ..., H_n\f$ such that if
/// \f$Q= H_1 H_2 ... H_n \f$, then \f$Q^T A = R \f$ is upper triangular. The
/// upper triangular part of A is overwritten by the upper triangular part of
/// R and components j+1:m of the jth Householder vector are stored in
/// \f$A(j+1:m, j), j<m\f$.
/// Matrix A must be stored column-wise.
/// 
/// @param[in,out]  a  The matrix \f$A\in{\Re}^{m\times n}\f$ (column-wise). At
///                    output it is overwritten by the upper triangular part of
///                    R and Householder vector components.
/// @param[in]      b  A vector of size n; at output, it contains the \f$\beta\f$
///                    coefficients for each of the n-1 Householder vectors.
/// @param[in]      m  Number of rows of a matrix A.
/// @param[in]      n  Number of columns of matrix A.
/// @param[out] sign   If non-zero, then the function has ended with error.
///
/// Reference: Matrix Computations, G.H. Colub, CF.V. Loan, 1996, pg. 224
void
householder_qr(double *__restrict__ a, double *__restrict__ b, int m, int n,
    int &sign)
noexcept
{
    double *u,sum;
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
        b[col] = householder_vec(&a[col*m+col], m-col, u);
        
        // Compute A(col:m, col:n) = (I-buu^T)A(col+1:m, col+1:n)
        for (j = col; j < n; j++) {
            for (sum = 0e0, row = col; row < m; row++) sum += a[j*m+row]*u[row-col];
            sum *= b[col];
            for (row = col; row < m; row++) a[j*m+row] -= sum*u[row-col];
        }

        // if (j < m) A(j+1:m, j) = u(2:m-j+1)
        if (col < m) {
            for (row = col+1; row < m; row++) a[col*m+row] = u[row-col];
        }
    }

    delete[] u;
    return;
}

/// @brief Compute the thin Q matrix of a QR factorization.
///
/// This function computes the thin Q matrix of a QR factorization, given that
/// the factorization is already performed (via householder_qr) and its result
/// is stored in the input matrix a.
/// The upper triangular part of a must hold the upper triangular part of
/// R and components \f$j+1:m\f$ of the jth Householder vector should be stored
/// in \f$a(j+1:m, j), j<m\f$ (this is exactly the output of householder_qr).
/// The result Q matrix, \f$Q \in {\Re}^{m \times n}\f$ is written in the
/// (input) q array (which should be of size m*n).
/// Note that this is the thin Q matrix, i.e. \f$Q_1 \in {\Re}^{m \times n}\f$
/// and \f$Q = \left( Q_1  Q_2 \right)\f$, \f$Q_2 \in {\Re}^{m \times (m-n)}\f$.
/// All matrices are supposed to be stored column-wise.
///
/// @param[in]     a  The result of a householder QR factorization (via 
///                   householder_qr). The size of this matrix is m*n.
/// @param[in]     b  A vector of size n; it contains the \f$\beta\f$ 
///                   coefficients for each of the n-1 Householder vectors
///                   (computed as part of the householder_qr algorithm).
/// @param[in,out] q  At output the m*n Q matrix
/// @param[in]     m  Number of rows of a and q matrices.
/// @param[in]     n  Number of columns of a and q matrices.
///
/// Reference: Matrix Computations, G.H. Colub, CF.V. Loan, 1996, pg. 213
void
thin_q(double *__restrict__ a, double *__restrict__ b, double *__restrict__ q,
    int m, int n)
{
    int i,j,col;
    double sum,*u;

    try {
        u = new double[m];
    } catch (std::bad_alloc&) {
        return;
    }

    std::fill(q, q+m*n, 0e0);
    for (col = 0; col < n; col++) q[col*m+col] = 1e0;

    for (j = n-1; j >= 0; j--) {
        u[j] = 1e0;
        for (i = j+1; i < m; i++) u[i] = a[j*m+i];
        for (col = j; col < n; col++) {
            for (sum = 0e0, i = j; i < m; i++) sum += u[i]*q[col*m+i];
            sum *= b[j];
            for (i = j; i < m; i++) q[col*m+i] -= sum*u[i];
        }
    }

    delete[] u;
    return;
}

/// @brief Householder-QR LS solution.
///
/// If \f$A \in {\Re}^{m \times n}\f$ has full comumn rank and 
/// \f$b \in {\Re}^m \f$, then this algorithm computes a vector \f$x_{LS}\f$
/// such that \f$ \| A x_{LS} - b \| \f$ is minimum.
/// More simply, this algorithm solves the (overdetermined) Least Squares
/// system \f$Ax=b\f$, via QR factorization, using the Householder algorithm.
///
/// @param[in]     a  The design or data matrix; should be of size mxn.
/// @param[in,out] b  The observation vector, of size m. At output, it is
///                   overwritten by the LS solution vector.
/// @param[in]     m  Number of observations (rows of a and b).
/// @param[in]     n  Number of parameters (columns of a).
///
/// Reference: Matrix Computations, G.H. Colub, CF.V. Loan, 1996, pg. 240
void
ls_qrsolve(double *__restrict__ a, double *__restrict__ b, int m, int n)
{
    double *beta, *u, sum;
    int i, j, sign;

    try {
        beta = new double[m];
        u    = new double[m];
    } catch (std::bad_alloc&) {
        return;
    }

    // Overwrite a with its QR factorization
    householder_qr(a, beta, m, n, sign);

    // Compute b <- (Q^T)*b
    for (j = 0; j < n; j++) {
        for (sum = b[j], i = j+1; i < m; i++) sum += a[j*m+i]*b[i];
        sum *= beta[j];
        b[j] -= sum;
        for (i = j+1; i < m; i++) b[i] -= sum*a[j*m+i];
    }

    // Solve R(1:n,1:n) * x = n(1:n)
    for (j = n-1; j > 0; j--) {
        b[j] /= a[j*m+j];
        for (i = 0; i < j-1; i++) b[i] -= b[j]*a[j*m+i];
    }
    b[0] /= a[0];

    delete[] u;
    delete[] beta;
    return;
}
