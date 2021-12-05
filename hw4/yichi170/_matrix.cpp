#include "_matrix.hpp"
#include <mkl.h>
#include <algorithm>
#include <iostream>

static CustomAllocator<double> alloc;

Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol), m_buffer(alloc) {
    reset_buffer(nrow, ncol);
}

Matrix::Matrix(const Matrix & other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
    
    reset_buffer(other.m_nrow, other.m_ncol);
    for (size_t i = 0; i < m_nrow; ++i)
        for (size_t j = 0; j < m_ncol; ++j)
            (*this)(i, j) = other(i, j);
}

Matrix::Matrix(Matrix && other)
    : m_nrow(std::exchange(other.m_nrow, 0))
    , m_ncol(std::exchange(other.m_ncol, 0))
    , m_buffer(std::move(other.m_buffer)) {}

Matrix & Matrix::operator=(const Matrix & other) {
        
    if (this == &other) { return *this; }
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        reset_buffer(other.m_nrow, other.m_ncol);
    
    for (size_t i = 0; i < m_nrow; i++)
        for (size_t j = 0; j < m_ncol; j++)
            (*this)(i, j) = other(i, j);
        
    return *this;
}

Matrix::~Matrix() {  reset_buffer(0, 0); }

bool Matrix::operator== (Matrix const & mt) const {
    if ((m_ncol != mt.m_ncol) && (m_nrow != mt.m_nrow))
        return false;

    for (size_t i = 0; i < m_nrow; i++)
        for (size_t j = 0; j < m_ncol; j++)
            if ((*this)(i, j) != mt(i, j))
                return false;
    
    return true;
}

inline size_t cmp(size_t a, size_t b) {
    if (a < b)
        return a;
    return b;
}

inline void validation_check(const Matrix &mt1, const Matrix &mt2) {
    if (mt1.m_ncol != mt2.m_nrow)
        throw std::out_of_range("THe size of Matrix 1 and Matrix 2 isn't matched");
}

Matrix multiply_naive(const Matrix &mt1, const Matrix &mt2) {

    validation_check(mt1, mt2);
    
    size_t m1r = mt1.m_nrow, m2c = mt2.m_ncol, m1c = mt1.m_ncol;
    Matrix ret(m1r, m2c);

    for (size_t i = 0; i < m1r; i++) {
        size_t retiBase = i * m2c, m1Base = i * m1c;
        for (size_t j = 0; j < m2c; j++) {
            size_t retBase = retiBase + j;
            for (size_t k = 0; k < m1c; k++) {
                ret.m_buffer[retBase] += mt1.m_buffer[m1Base + k] * mt2.m_buffer[k * m2c + j];
            }
        }
    }
    return ret;
}

Matrix multiply_tile(const Matrix &mt1, const Matrix &mt2, size_t t_size) { // base on size
    
    validation_check(mt1, mt2);
    
    size_t m1r = mt1.m_nrow, m2c = mt2.m_ncol, m1c = mt1.m_ncol;
    Matrix ret(m1r, m2c);

    for (size_t i = 0; i < m1r; i += t_size)
        for (size_t j = 0; j < m2c; j += t_size)
            for (size_t k = 0; k < m1c; k += t_size) {
                size_t ibound = cmp(m1r, i+t_size), jbound = cmp(m2c, j+t_size), kbound = cmp(m1c, k+t_size);
                for (size_t kk = k; kk < kbound; kk++) {
                    size_t m2Base = kk * m2c;
                    for (size_t ii = i; ii < ibound; ii++) {
                        size_t retiBase = ii * m2c, m1Base = ii * m1c + kk;
                        for (size_t jj = j; jj < jbound; jj++) {
                            ret.m_buffer[retiBase + jj] += mt1.m_buffer[m1Base] * mt2.m_buffer[m2Base + jj];
                        }
                    }
                }
            }
    return ret;
}

Matrix multiply_ntile(const Matrix &mt1, const Matrix &mt2, size_t n_tile) { // base on the number of tile
    
    validation_check(mt1, mt2);
    
    size_t m1r = mt1.m_nrow, m2c = mt2.m_ncol, m1c = mt1.m_ncol;
    size_t m1rt = mt1.m_nrow / n_tile, m2ct = mt2.m_ncol / n_tile, m1ct = mt1.m_ncol / n_tile;
    Matrix ret(m1r, m2c);

    for (size_t i = 0; i < n_tile; i++) {
        for (size_t j = 0; j < n_tile; j++) {
            for (size_t k = 0; k < n_tile; k++) {
                // (i, k) * (k, j)
                size_t iilbound = i*m1rt, jjlbound = j*m2ct, kklbound = k*m1ct;
                size_t iiubound = iilbound+m1rt, jjubound = jjlbound+m2ct, kkubound = kklbound+m1ct;
                
                for (size_t ii = iilbound; ii < iiubound; ii++) {
                    size_t retiBase = ii * m2c, m1Base = ii * m1c;
                    for (size_t jj = jjlbound; jj < jjubound; jj++) {
                        size_t retBase = retiBase + jj;
                        for (size_t kk = kklbound; kk < kkubound; kk++)
                            ret.m_buffer[retBase] += mt1.m_buffer[m1Base + kk] * mt2.m_buffer[kk * m2c + jj];
                    }
                }
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(const Matrix &mt1, const Matrix &mt2) {
    
    validation_check(mt1, mt2);
    
    Matrix ret(mt1.m_nrow, mt2.m_ncol);

    cblas_dgemm(
        CblasRowMajor     /* const CBLAS_LAYOUT Layout    */
      , CblasNoTrans      /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans      /* const CBLAS_TRANSPOSE transb */
      , mt1.m_nrow        /* const MKL_INT m              */
      , mt2.m_ncol        /* const MKL_INT n              */
      , mt1.m_ncol        /* const MKL_INT k              */
      , 1.0               /* const double alpha           */
      , &mt1.m_buffer[0]  /* const double *a              */
      , mt1.m_ncol        /* const MKL_INT lda            */
      , &mt2.m_buffer[0]  /* const double *b              */
      , mt2.m_ncol        /* const MKL_INT ldb            */
      , 0.0               /* const double beta            */
      , &ret.m_buffer[0]  /* double * c                   */
      , ret.m_ncol        /* const MKL_INT ldc            */
    );
    
    return ret;
}

std::size_t bytes() { return alloc.bytes(); }
std::size_t allocated() { return alloc.allocated(); }
std::size_t deallocated() { return alloc.deallocated(); }
