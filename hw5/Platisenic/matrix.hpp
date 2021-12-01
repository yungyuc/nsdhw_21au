#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <tuple>
#include <mkl.h>


class Matrix{
public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol){
        if (size()) { m_buffer = new double[size()]; }
        else { m_buffer = nullptr; }
        for (size_t i = 0; i < size(); ++i) { m_buffer[i] = 0; }
    }

    // ~Matrix(){
    //     delete [] m_buffer;
    // }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t size() const { return m_nrow * m_ncol; }

    size_t index(size_t row, size_t col) const{
        return row * m_ncol + col;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;
};

bool operator==(Matrix const &lhs, Matrix const &rhs){
    if((lhs.m_nrow != rhs.m_nrow) && (lhs.m_ncol != rhs.m_ncol)) { return false; }

    for(size_t i=0; i<lhs.m_ncol; i++){
        for(size_t j=0; j<lhs.m_nrow; j++){
            if(lhs(i, j) != rhs(i, j)) { return false; }
        }
    }
    return true;
}

bool operator!=(Matrix const &lhs, Matrix const &rhs){
    return !(lhs == rhs);
}

Matrix multiply_naive(const Matrix &m1, const Matrix &m2){
    Matrix m3(m1.nrow(), m2.ncol());
    for(size_t i=0; i<m1.nrow(); i++){
        for(size_t j=0; j<m2.ncol(); j++){
            double t = 0;
            for(size_t k=0; k<m1.nrow(); k++){
                t += m1(i, k) * m2(k, j);
            }
            m3(i, j) = t;
        }
    }
    return m3;
}

Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t blocksize){
    Matrix m3(m1.nrow(), m2.ncol());

    for (size_t i=0; i<m1.nrow(); i+=blocksize){
        for (size_t k=0; k<m2.ncol(); k+=blocksize){
            for (size_t j=0; j<m1.ncol(); j+=blocksize){
                 for (size_t kk = j; kk < std::min(m1.ncol(), j + blocksize); kk++){
                    size_t m2base = kk*m2.nrow();
                    for (size_t ii = i; ii < std::min(m1.nrow(), i + blocksize); ii++) {
                        size_t m1base = ii*m1.nrow()+kk;
                        size_t m3base = ii*m3.nrow();
                        for (size_t jj =k; jj < std::min(m2.ncol(), k + blocksize); jj++) {
                            // m3(ii, jj) += m1(ii, kk) * m2(kk, jj);
                            m3.m_buffer[m3base+jj] += m1.m_buffer[m1base] * m2.m_buffer[m2base+jj];
                        }
                    }
                }
            }
        }
    }
    return m3;
}

Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2){
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
      , &mat1.m_buffer[0] /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , &mat2.m_buffer[0] /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , &ret.m_buffer[0] /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}
