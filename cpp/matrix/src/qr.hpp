/// @brief This file contains algorithms connected to QR factorization.
///
/// @note
///     - All matrix/vector indexes start from 0.
///     - Unless otherwise stated, all matrices are stored in column-major form.
///

///
/// @mainpage
///
/// @section sec-1 QR decomposition
/// The QR factorization of an m-by-n matrix A, is given by: \f$A=QR\f$, where
/// \f$Q\in {\Re}^{m\times m}\f$ is orthogonal and \f$R\in {\Re}^{m\times n}\f$
/// is upper triangular. In the following we assume \f$m \ge n\f$. If A has
/// full column rank, then the first n columns of Q form an orthonormal basis
/// for ran(A). The computation of the factorization can be arranged in several
/// ways including e.g. Householder, Givens, etc.
/// As the bottom (m−n) rows of an m×n upper triangular matrix consist entirely
/// of zeroes, it is often useful to partition R, or both R and Q:
/// \f[ A=QR=Q\left( \begin{array}{c} R_1 \\ 0 \end{array} \right) = \left( Q_1  Q_2 \right) \left( \begin{array}{c} R_1 \\ 0 \end{array} \right) = Q_1 R_1 \f]
/// where \f$R_1 \f$ is an \f$n \times n\f$ upper triangular matrix, \f$0\f$ 
/// is an \f$(m-n) \times n\f$ zero matrix, \f$Q_1 \f$ is \f$m \times n\f$, 
/// \f$Q_2 \f$ is \f$m\times (m-n)\f$, and \f$Q_1 \f$ and \f$Q_2 \f$ both have 
/// orthogonal columns.
/// \f$Q_1 R_1 \f$ is sometimes called the thin QR factorization of A.
///
/// @section sec-2 QR decomposition and Least Squares
/// QR factorization can be used to solve LS problems. Consider the data matrix
/// \f$A\in {\Re}^{m\times n}\f$ and the observation vector \f$b\in {\Re}^m\f$
/// and \f$m > n\f$. Suppose that an orthogonal matrix \f$Q\in {\Re}^{m\times m}\f$
/// has been computed such that
/// \f[Q^T A=R=\left( \begin{array}{c} {R_1}^{n\times n} \\ 0^{m-n\times n} \end{array} \right)\f]
/// is upper triangular. If
/// \f[Q^T b=\left( \begin{array}{c} c^{n\times 1} \\ d^{m-n\times 1} \end{array} \right)\f]
/// then \f[ {\|Ax-b\|}^2 = {\|Q^T Ax- Q^T b\|}^2 = {\|R_1 x-c\|}^2 + {\|d\|}^2\f]
/// for any \f$x \in {\Re}^n\f$. If \f$rank(A) = rank(R_1 ) = n\f$, then \f$x_{LS}\f$ is defined
/// by the upper triangular system \f$R_1 x_{LS} = c\f$. 
///

void
back_substitution(const double *__restrict__ u, double *__restrict__ b, int n)
noexcept;

double
householder_vec(const double *__restrict__ x, int n, double *__restrict__ u)
noexcept;

void
householder_qr(double *__restrict__ a, double *__restrict__ b, int m, int n, int &sign)
noexcept;

void
ls_qrsolve(double *__restrict__ a, double *__restrict__ b, int m, int n);

void 
qrdcmp(double *__restrict__ a, double *__restrict__ c, double *__restrict__ d,
    int n, int &sign)
noexcept;

void
thin_q(double *__restrict__ a, double *__restrict__ b, double *__restrict__ q, int m, int n);
