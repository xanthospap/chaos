#include <iostream>

#ifdef DEBUG
#include <iostream>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#endif

#include "qr.hpp"
#include "eigen3/Eigen/Core"
#include "eigen3/Eigen/QR"

using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using namespace std::literals::chrono_literals;

int main()
{
    int rows {10},
        cols {4},
        sign;
    
    // A random matrix A (column-major form)
    double A_cw[] = {
                  1.0e0,  -4.0e0,   7.0e0,   3.0e0,
                -10.0e0,  2.0e0,  -2.0e0,  -7.0e0,
                  8.0e0,  -1.0e0,  11.0e0,   8.0e0,
                  6.0e0,   6.0e0,  -5.0e0, -12.0e0,
                -17.0e0,   0.4e0,  -5.0e0,   3.0e0,
                 -7.0e0,   5.2e0,  -5.2e0,  14.0e0,
                  3.4e0,  -2.0e0,   7.0e0,  -7.0e0,
                121.0e0,   9.0e0, -32.0e0,   2.0e0,
                 32.0e0,  10.0e0,  -9.0e0,  77.0e0,
                -69.0e0,  11.0e0,   4.0e0,   9.5e0};

    // A copy of A, in Eigen format (row-major form)
    Eigen::MatrixXd A_rw = Eigen::MatrixXd(rows, cols);
    for (int i=0; i<rows; i++) for (int j=0; j<cols; j++) A_rw(i,j) = A_cw[j*rows+i];

    // Print matrix A
    printf("\nMatrix A:\n");
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            printf("%+15.10f ",A_cw[j*rows+i]);
        }
        printf("\n");
    }
    
    // Get its QR factorization via householder_qr
    double beta[rows];
    householder_qr(A_cw, beta, rows, cols, sign);
    printf("\nMatrix A after householder_qr(...) :\n");
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            printf("%+20.15f ",A_cw[j*rows+i]);
        }
        printf("\n");
    }
    
    // get the thin-Q matrix
    double qq[cols*rows];
    printf("\n");
    thin_q(A_cw, beta, qq, rows, cols);
    printf("\nMatrix Q after householder_qr(...) :\n");
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            printf("%+20.15f ",qq[j*rows+i]);
        }
        printf("\n");
    }

    // Perform QR via Eigen
    Eigen::IOFormat fmt(15);
    Eigen::MatrixXd thinQ (Eigen::MatrixXd::Identity(rows,cols)),
                    Q;
    Eigen::HouseholderQR<Eigen::MatrixXd> qr(A_rw);
    Q     = qr.householderQ();
    thinQ = qr.householderQ() * thinQ;
    Eigen::MatrixXd R  = qr.matrixQR().template  triangularView<Eigen::Upper>();
    Eigen::MatrixXd QR = qr.matrixQR();
    std::cout << "\nThe A matrix is:\n" << QR.format(fmt) << "\n\n";
    // std::cout << "The R (upper triangular) matrix is:\n" << R << "\n\n";
    // std::cout << "The complete unitary matrix Q is:\n" << Q.format(fmt) << "\n\n";
    std::cout << "The thin matrix Q is:\n" << thinQ.format(fmt) << "\n\n";
    // std::cout << "Householder factors:\n" << qr.hCoeffs() << "\n\n";
    
    printf("\n-----------------------------------------------------------------------------------");
    printf("\nLeast Squares via QR");
    printf("\n-----------------------------------------------------------------------------------");
    // Solve an LS system
    double C[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                -10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,
                100, 81, 64, 49, 36, 25, 16,9,4,1,0,1,4,9,16,25,36,49,64,81,100};
    double y[] = {-35.84000,-36.27000,-35.78800,-34.39400,-32.08800,-28.87000,-24.74000,-19.69800,-13.74400, -6.87800,  0.90000,  9.59000, 19.19200, 29.70600, 41.13200, 53.47000, 66.72000, 80.88200, 95.95600,111.94200,128.84000};

    printf("\nMatrix A\n");
    for (int i=0; i<21; i++) {
        for (int j=0; j<3; j++) {
            printf("%+10.5f ",C[j*21+i]);
        }
        printf("\n");
    }
    printf("\nVector b\n");
    for (int i=0; i<21; i++) {
            printf("%+10.5f ",y[i]);
    }
    
    ls_qrsolve(C, y, 21, 3);
    printf("\n-- QR Solve -- (mine)");
    for (int i=0; i<3; i++) printf("\n\tx[%1d] = %+7.3f", i+1, y[i]);
    Eigen::MatrixXd C_rw(21, 3);
    Eigen::VectorXd y_rw(21), x_rw(21);
    for (int i=0; i<21; i++) {
        y_rw(i) = y[i];
        for (int j=0; j<3; j++) {
            C_rw(i,j) = C[j*21+i];
        }
    }
    x_rw = C_rw.householderQr().solve(y_rw);
    printf("\n-- QR Solve -- (eigen)");
    std::cout<< "\n" << x_rw << "\n";

    /*   
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_real_distribution<double> distr(-125e0, 125e0); // define the range
    
    int ROWS = 4, COLS = 4;
    double A[] = {1.0e0,  -4.0e0,   7.0e0,   3.0e0,
                 -10.0e0,  2.0e0,  -2.0e0,  -7.0e0,
                  8.0e0,  -1.0e0,  11.0e0,   8.0e0,
                  6.0e0,   6.0e0,  -5.0e0, -12.0e0};
    double A_copy[ROWS*COLS];
    for (int i=0; i<ROWS*COLS; i++) A_copy[i] = A[i];
    double A_rw[ROWS*ROWS];
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            A_rw[i*COLS+j] = A[j*ROWS+i];
        }
    }

    printf("\nMatrix A:\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",A_copy[j*ROWS+i]);
        }
        printf("\n");
    }
    
    int sign;
    double c[ROWS], d[ROWS];
    householder_qr(A_copy,c,ROWS, COLS, sign);
    printf("\nMatrix A after householder_qr(...) :\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",A_copy[j*ROWS+i]);
        }
        printf("\n");
    }
    
    printf("\nMatrix A:\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",A_rw[i*COLS+j]);
        }
        printf("\n");
    }
    
    qrdcmp(A_rw,c,d,ROWS,sign);
    printf("\nMatrix A after qrdcmp(...) :\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",A_rw[i*COLS+j]);
        }
        printf("\n");
    }
    
    ROWS = 10;
    COLS = 4;
    double B[] = {1.0e0,  -4.0e0,   7.0e0,   3.0e0,
                 -10.0e0,  2.0e0,  -2.0e0,  -7.0e0,
                  8.0e0,  -1.0e0,  11.0e0,   8.0e0,
                  6.0e0,   6.0e0,  -5.0e0, -12.0e0,
                -17.0e0,   0.4e0,  -5.0e0,   3.0e0,
                 -7.0e0,   5.2e0,  -5.2e0,  14.0e0,
                  3.4e0,  -2.0e0,   7.0e0,  -7.0e0,
                121.0e0,   9.0e0, -32.0e0,   2.0e0,
                 32.0e0,  10.0e0,  -9.0e0,  77.0e0,
                -69.0e0,  11.0e0,   4.0e0,   9.5e0};
    double B_copy[ROWS*COLS];
    for (int i=0; i<ROWS*COLS; i++) B_copy[i] = B[i];
    
    printf("\nMatrix B :\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",B[j*ROWS+i]);
        }
        printf("\n");
    }

    double betas[COLS];
    householder_qr(B, betas, ROWS, COLS, sign);
    printf("\nMatrix B after householder_qr(...) :\n");
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%+15.10f ",B[j*ROWS+i]);
        }
        printf("\n");
    }

    double C[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                    -10,-9,-8,-7,-6,-5,-4,-3,-2,-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,
                    100, 81, 64, 49, 36, 25, 16,  9,  4,  1,  0,  1,  4,  9, 16, 25, 36, 49, 64, 81,100};
    double y[] = {-35.84000,-36.27000,-35.78800,-34.39400,-32.08800,-28.87000,-24.74000,-19.69800,-13.74400, -6.87800,  0.90000,  9.59000, 19.19200, 29.70600, 41.13200, 53.47000, 66.72000, 80.88200, 95.95600,111.94200,128.84000};

    ls_qrsolve(C, y, 21, 3);
    printf("\n-- QR Solve --");
    for (int i=0; i<3; i++) printf("\n\tx[%1d] = %+7.3f", i+1, y[i]);
    */
    
    /*
    time_point<Clock> start,
                      end;
    milliseconds      diff;
    for (int size = 10; size < 2000; size += 300) {
        double MATRIX[size*size], C[size], D[size];
        ROWS = COLS = size;
        for (int i=0; i<size*size; i++) MATRIX[i] = distr(eng);

        start = Clock::now();
        householder_qr(MATRIX, C, size, size, sign);
        end = Clock::now();
        diff = duration_cast<milliseconds>(end - start);
        std::cout << size << " householder_qr: " << diff.count();
        
        start = Clock::now();
        qrdcmp(MATRIX, C, D, size, sign);
        end = Clock::now();
        diff = duration_cast<milliseconds>(end - start);
        std::cout << " qrdcmp: " << diff.count() << "ms\n";
    }
    */

    printf("\n");
    return 0;
}
