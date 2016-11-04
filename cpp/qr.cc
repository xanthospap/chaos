#include <iostream>
#include <cassert>
#include <cmath>

/// WARNING
/// ---------------------------------------------
/// * All matrix/vector indexes start from 0.
/// * All matrix/vector are stored column-wise
///

/// TODO
/// * operator new should be in a try/catch block
///

struct submat;

struct mat
{
    double*     _dptr;
    std::size_t _rows,
                _cols,
                _srows, // only for submatrices; real number of rows
                _scols; // only for submatrices; real number of cols

    explicit mat(double* p = nullptr, std::size_t r = 0, std::size_t c = 0,
        std::size_t sr = 0, std::size_t sc = 0)
    : _dptr(p), _rows(r), _cols(c), _srows(sr), _scols(sc)
    {}

    double&
    operator()(std::size_t r, std::size_t c)
    {
        assert( c*_rows+r < _rows * _cols );
        return _dptr[c*_rows+r];
    }

    double
    operator()(std::size_t r, std::size_t c) const
    {
        assert( c*_rows+r < _rows * _cols );
        return _dptr[c*_rows+r];
    }

    mat
    submatrix(std::size_t row1, std::size_t row2, std::size_t col1, std::size_t col2)
    {
        assert( (row2 >= row1) && (row2 <= this->_rows) );
        assert( (col2 >= col1) && (col2 <= this->_cols) );

        std::size_t nr { (row2 == row1) ? 1 : (row2-row1) };
        std::size_t nc { (col2 == col1) ? 1 : (col2-col1) };

        std::cout<<"\n\tSubMatrix info: rows: "<< nr <<", cols: "<< nc <<"\n";
        
        return mat{&(this->operator()(row1, col1)), _rows, _cols, nr, nc};
    }

    void
    dump() const
    {
        std::size_t real_rows = _srows ? _srows : _rows;
        std::size_t real_cols = _scols ? _scols : _cols;
        
        for (std::size_t i = 0; i < real_rows; ++i)
        {
            for (std::size_t j = 0; j < real_cols; ++j)
            {
                std::cout<< " " << this->operator()(i, j);
            }
            std::cout<<"\n";
        }
    }
};


int main()
{
    std::size_t ROWS{5}, COLS{4};
    double* nums = new double[ROWS*COLS];
    double num{0};

    for (std::size_t r=0;r<ROWS;++r)
        for (std::size_t c=0;c<COLS;++c)
            nums[r*COLS+c] = num++;

    mat m{nums, ROWS, COLS};
    std::cout<<"\nOriginal matrix size: "<<m._rows<<"x"<<m._cols<<"\n";
    m.dump();

    std::cout<<"Submatrix A(3:, 2:) is:\n";
    auto m1 = m.submatrix(3, ROWS, 2, COLS);
    m1.dump();
    
    std::cout<<"Submatrix A(3:, 2) is:\n";
    m1 = m.submatrix(3, ROWS, 2, 2);
    m1.dump();
    
    std::cout<<"Submatrix A(1:4, 1) is:\n";
    m1 = m.submatrix(1, 4, 1, 1);
    m1.dump();

    return 0;
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
    double* t = new double[n*m];
    double* Ajj;
    std::size_t idx;

    for (int j = 0; j < n; ++j) {
        Ajj = &A[m*j]+j;
        b   = householder_vector(Ajj, m-j+1, u);
        // A(j:m, j:n) = (I_(m-j+1)-buu^T)A(j:m,j:n)
        // compute I_(m-j+1)-buu^T <-- t
        idx = 0;
        for (std::size_t ii = 0; ii < m-j+1; ii++) {
            for (std::size_t jj = 0; jj < ii; jj++) {
                t[idx] = t[ii*(m-j+1)+jj];
                ++idx;
            }
            t[idx] = 1e0 - b*u[ii]*u[ii];
            for (std::size_t jj = ii+1; jj < m-j+1; jj++) {
                idx++;
                t[idx] = b*u[ii]*u[jj];
            }
        }
    }
}
