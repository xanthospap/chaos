/// \brief LU decomposition of a square matrix.
///
/// This function computes the LU decomposition of a square matrix, using an
/// outer-product Gaussian elimination algorithm.
/// Suppose \f$A \in \Re^{x\times n} \f$ has the property that \f$A(1:k, 1:k)\f$
/// is non-singular for \f$k=1:n-1\f$. This algorithm computes the factorization
/// \f$M_{n-1}...M_{1}A=U\f$ where \f$U\f$ is upper triangular and each \fM_k \f$
/// is a Gauss transform. \f$U\f$ is stored in the upper triangle of A. The
/// multipliers associated with \f$M_k \f$ are stored in
/// \f$A(k+1:n,k), i.e., A(k+1:n,k)=-M_{k}(k:1:n,k)\f$.
///
/// \param[in,out] a  The matrix A of size nxn at input; at output, its upper
///                   triangle is overwritten by U.
/// \param[in]     n  Dimension of A; A has size nxn.
///
void
square_lu(double *__restrict__ a, int n)
{
    int k,i,j;
    for (k = 0; k < n-1; k++) {
        for (i = k+1; i < n; i++) a[i*n+k] /= a[k*n+k];
        for (j = k+1; j < n; j++) {

}
