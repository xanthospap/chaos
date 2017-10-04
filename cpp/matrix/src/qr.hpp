/// \brief This file contains algorithms connected to QR factorization.
///
/// \notes
///     - All matrix/vector indexes start from 0.
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
