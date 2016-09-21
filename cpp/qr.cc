#include <iostream>

/// WARNING
/// ---------------------------------------------
/// * All matrix/vector indexes start from 0.
/// * All matrix/vector are stored column-wise
///

/// TODO
/// * operator new should be in a try/catch block
///

/// Van Golub, Matrix Computations, p. 210
/// Algorithm 5.1.1
double*
householder_vector(const double* A, int hcol, int rows, int cols, double& beta)
{
    // ptr to the vector we want
    double* vec = &A[hcol*rows];

    // ptr to the new householder vector
    double* u = nullptr;
    u = new double[rows];
    *u = 1.0e0;

    double sigma{0};
    for (int i = 1; i < rows; ++i) {
        sigma += vec[i]*vec[i];
        u[i]   = vec[i];
    }

    if ( sigma == 0e0 ) {
        beta = 0e0;
    } else {
        double m = std::sqrt(vec[0]*vec[0]+sigma);
        if ( vec[0]<= 0e0 ) {
            u[0] = vec[0] - m;
        } else {
            u[0] = -1e0 * sigma / (vec[0] + m);
        }
        beta = 2e0*u[0]*u[0] / (sigma+u[0]*u[0]);
    }

    double tmp = *u;
    for (int i = 1; i < rows; ++i) u[i] /= tmp;

    return u;
}

void
qr_decomposition(const double* A, int rows, int cols)
{

}
