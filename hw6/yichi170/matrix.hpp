#include <stdexcept>
#include <cstddef>
#include <mkl.h>
#include <functional>

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol) {
        reset_buffer(nrow, ncol);
    }

    Matrix(const Matrix & other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
        
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i)
            for (size_t j = 0; j < m_ncol; ++j)
                (*this)(i, j) = other(i, j);
    }

    Matrix & operator=(const Matrix & other) {
        
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
            reset_buffer(other.m_nrow, other.m_ncol);
        
        for (size_t i = 0; i < m_nrow; i++)
            for (size_t j = 0; j < m_ncol; j++)
                (*this)(i, j) = other(i, j);
            
        return *this;
    }

    ~Matrix() {  delete[] m_buffer; }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double *buffer() { return m_buffer; }
    
    bool operator== (Matrix const & mt) const {
        if ((m_ncol != mt.m_ncol) && (m_nrow != mt.m_nrow))
            return false;

        for (size_t i = 0; i < m_nrow; i++)
            for (size_t j = 0; j < m_ncol; j++)
                if ((*this)(i, j) != mt(i, j))
                    return false;
        
        return true;
    }

    size_t index(size_t row, size_t col) const { return row * m_ncol + col; }

    void reset_buffer(size_t nrow, size_t ncol) {
        if (m_buffer) {
            delete[] m_buffer;
        }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        for (size_t i = 0; i < nelement; i++)   m_buffer[i] = 0;
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;
};

inline size_t cmp(size_t a, size_t b) {
    if (a < b)
        return a;
    return b;
}

Matrix multiply_naive(const Matrix &mt1, const Matrix &mt2) {
    
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

Matrix multiply_mkl(const Matrix &mt1, const Matrix &mt2) {

    Matrix ret(mt1.m_nrow, mt2.m_ncol);

    cblas_dgemm(
        CblasRowMajor     /* const CBLAS_LAYOUT Layout    */
      , CblasNoTrans      /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans      /* const CBLAS_TRANSPOSE transb */
      , mt1.m_nrow        /* const MKL_INT m              */
      , mt2.m_ncol        /* const MKL_INT n              */
      , mt1.m_ncol        /* const MKL_INT k              */
      , 1.0               /* const double alpha           */
      , mt1.m_buffer      /* const double *a              */
      , mt1.m_ncol        /* const MKL_INT lda            */
      , mt2.m_buffer      /* const double *b              */
      , mt2.m_ncol        /* const MKL_INT ldb            */
      , 0.0               /* const double beta            */
      , ret.m_buffer      /* double * c                   */
      , ret.m_ncol        /* const MKL_INT ldc            */
    );
    
    return ret;
}
