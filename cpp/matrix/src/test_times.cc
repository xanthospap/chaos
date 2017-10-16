
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
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_real_distribution<double> distr(-125e0, 125e0); // define the range
    std::vector<int> row_vec = {10, 100, 1000, 2000, 3000, 5000, 10000};
    std::vector<int> col_vec = {5, 10, 20};
    time_point<Clock> start,
                      end;
    milliseconds      diff;
    
    double *A, *b, *c;
    int    sign;
    for (int cols : col_vec) {
        for (int rows : row_vec) {
            if (rows > cols) {
                A = new double[cols*rows];
                // b = new double[rows];
                c = new double[cols];
                for (int i = 0; i < rows*cols; i++) A[i] = distr(eng);
                // for (int i = 0; i < rows; i++) b[i] = distr(eng);

                start = Clock::now();
                householder_qr(A, c, rows, cols, sign);
                end = Clock::now();
                diff = duration_cast<milliseconds>(end - start);
                printf("\n%5d x %5d", rows, cols);
                std::cout << " mine: " << diff.count();

                delete[] A;
                delete[] c;

                Eigen::MatrixXd Agn = Eigen::MatrixXd(rows, cols);
                for (int i = 0; i < rows; i++)
                    for (int j = 0; j < cols; j++)
                        Agn(i,j) = distr(eng);
                start = Clock::now();
                Eigen::HouseholderQR<Eigen::MatrixXd> qr(Agn);
                end = Clock::now();
                diff = duration_cast<milliseconds>(end - start);
                std::cout << " Eigen: " << diff.count();
            }
        }
    }

    std::cout << "\n";
    return 0;
}
