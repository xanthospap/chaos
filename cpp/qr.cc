#include <iostream>

/// WARNING
/// ---------------------------------------------
/// * All matrix/vector indexes start from 0.
/// * All matrix/vector are stored column-wise
///

/// TODO
/// * operator new should be in a try/catch block
///

/// \brief Resolve sub-matrix
///
/// Given a matrix A with dimensions mxn, if we are interested in the sub-matrix
/// A(j:m, i:n), then this function will provide a pointer to the first element
/// of this (sub-)matrix and set its rows and columns.
///
double*
submatrix(const double* A, int m, int n, int j, int i, int& mm, int& nn)
{
    mm = m - j;
    nn = n - i;
    return &A[m*i]+j;
}

///
/// \brief Householder Vector
///
/// Given the vector x (of dimension n), this function computes the vector u 
/// (of dimension n) with u(1) = 1.0 and b (scalar) such that P = I_n - b*u*u^T
/// is orthogonal and Px = norm(x)e_1
///
/// Reference: Van Golub, Matrix Computations, p. 210, Algorithm 5.1.1
///
/// Modifications:
/// Instead of giving the vector x, we pass in the A matrix (of dimensions nxm),
/// of wich x is the col^th column.
///
/// \param[out] u    The Householder vector; the array should have a size of
///                  >= n (the function will overwrite u[0:n)).
/// \return          The scalar b (see description).
///
double
householder_vector(const double* x, int n, double* u)
{
    // ptr to the new householder vector
    *u = 1.0e0;

    double sigma = 0e0;
    for (int i = 1; i < n; ++i) {
        sigma += x[i] * x[i];
        u[i]   = x[i];
    }

    double beta = 0e0;
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

    double tmp = *u;
    for (int i = 1; i < n; ++i) u[i] /= tmp;

    return beta;
}

///
/// \brief QR decomposition via Householder.
///
/// Given A with dimensions mxn, where m>=n, the following algorithm finds
/// Householder matrices H_1, H_2, ..., H_n such that if Q = H_1H_2...H_n
/// then Q^TA=R is upper triangular. The upper triangular part of A is
/// overwritten by the upper triangular part of R and components j+1:m of the
/// j^th Householder vector are stored in A(j+1:m, j), j<m.
///
/// Reference: Van Golub, Matrix Computations, p. 224, Algorithm 5.2.1
///
/// Modifications:
/// Instead of giving the vector x, we pass in the A matrix (of dimensions nxm),
/// of wich x is the col^th column.
///
/// \param[in]  A    A matrix of dimenion nxm, where m>n.
/// \param[in]  m    Number of rows of matrix A.
/// \param[in]  n    Number of cols of matrix A.
/// \param[out] beta The scalar b (see description).
/// \return
///
void
householder_qr(double* A, int n, int m)
{
    double b;
    double* u = new double[m];
    double* Ajj;

    for (int j = 0; j < n; ++j) {
        Ajj = &A[m*j]+j;
        b   = householder_vector(Ajj, m-j);
        // A(j:m, j:n) = (I_(m-j+1)-buu^T)A(j:m,j:n)
    }
}
