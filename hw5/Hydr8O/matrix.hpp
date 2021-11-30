#include <mkl.h>


#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <functional>


struct Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement]();
    }

    Matrix(const Matrix & M)
    {
        m_nrow = M.m_nrow;
        m_ncol = M.m_ncol;
        size_t nelement = m_nrow * m_ncol;
        m_buffer = new double[nelement](); 
        memcpy(m_buffer, M.m_buffer, nelement*sizeof(double));
    }

    Matrix & operator=(const Matrix & other)
    {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        m_elapsed = other.m_elapsed;
        m_nflo = other.m_nflo;
        return *this;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }
    bool operator==(const Matrix &other) const {
        if ((ncol() !=other.ncol()) && (nrow() !=other.ncol())) {
            return false;
        }

        for (size_t i=0; i<nrow(); ++i) {
            for (size_t j=0; j<ncol(); ++j) {
                if (m_buffer[index(i, j)] != other(i, j)) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(const Matrix &other) const {
        if ((ncol() !=other.ncol()) && (nrow() !=other.ncol())) {
            return true;
        }

        for (size_t i=0; i<nrow(); ++i) {
            for (size_t j=0; j<ncol(); ++j) {
                if (m_buffer[index(i, j)] != other(i, j)) {
                    return true;
                }
            }
        }

        return false;
    }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

    double   elapsed() const { return m_elapsed; }
    double & elapsed()       { return m_elapsed; }

    size_t   nflo() const { return m_nflo; }
    size_t & nflo()       { return m_nflo; }

    double gflops() const { return m_nflo / m_elapsed / 1.e9; }

    Matrix transpose() const;

public:

    size_t index(size_t row, size_t col) const
    {
        return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    void set_item(std::tuple<int, int> dimensions, double value) {
        m_buffer[index(std::get<0>(dimensions), std::get<1>(dimensions))] = value;
    }

    double get_item(std::tuple<int, int> dimensions) {
        return m_buffer[index(std::get<0>(dimensions), std::get<1>(dimensions))];
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;
    double m_elapsed = 0;
    size_t m_nflo = 0; // number of floating-point operations.

};

Matrix Matrix::transpose() const {
    Matrix ret(nrow(), ncol());

    for (size_t i=0; i<ret.nrow(); ++i) {
        for (size_t j=0; j<ret.ncol(); ++j) {
            ret(j, i) = (*this)(i, j);
        }
    }
    return ret;
}


void initialize(Matrix & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            if (0 == i%2)
            {
                mat(i, j) = j;
            }
            else
            {
                mat(i, j) = mat.ncol() - 1 - j;
            }
        }
    }
}

// bool compare (Matrix const & mat1, Matrix const & mat2) {
//     if ((mat1.ncol() != mat2.ncol()) && (mat1.nrow() != mat2.ncol())) {
//         return false;
//     }

//     for (size_t i=0; i<mat1.nrow(); ++i) {
//         for (size_t j=0; j<mat1.ncol(); ++j) {
//             if (mat1(i, j) != mat2(i, j)) {
//                 return false;
//             }
//         }
//     }

//     return true;
// }

size_t calc_nflo(Matrix const & mat1, Matrix const & mat2)
{
    return mat1.nrow() * mat1.ncol() * mat2.ncol();
}

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2) {
    Matrix resultingMatrix(mat1.nrow(), mat2.ncol());

    const size_t nrow = mat1.nrow();
    const size_t ncol = mat1.ncol();
    const size_t ncol2 = mat2.ncol();
    for (size_t rowIdx = 0; rowIdx < nrow; ++rowIdx) {
        for (size_t columnIdx = 0; columnIdx < ncol2; ++columnIdx) {
            double rowColumnResult = 0;
            for (size_t column = 0; column < ncol; ++column) {
                rowColumnResult += mat1(rowIdx, column) * mat2(column, columnIdx);
            }
            resultingMatrix(rowIdx, columnIdx) = rowColumnResult;
        }
    }

    
    return resultingMatrix;
}

Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tile_size) {
    Matrix resultingMatrix(mat1.nrow(), mat2.ncol());

    const size_t nrow = mat1.nrow();
    const size_t ncol = mat1.ncol();

    for (size_t i = 0; i < nrow; i += tile_size) {
         for (size_t j = 0; j < ncol; j += tile_size) {
             resultingMatrix[i*ncol + j] = 0;
             for (size_t k = 0; k < nrow; k += tile_size) {
                 for (size_t x = i; x < std::min(i + tile_size, nrow); x++) {
                     for (size_t y = j; y < std::min(j + tile_size, ncol); y++) {
                         for (size_t z = k; z < std::min(k + tile_size, nrow); z++) {
                             resultingMatrix[ x * ncol + y ] +=  mat1[ x * ncol + z ] * mat2[ z * ncol  + y  ];
                         }
                     } 
                 }
             }
         }
     }

   
    return resultingMatrix;
}

Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2) {
    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());


    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.m_buffer /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}