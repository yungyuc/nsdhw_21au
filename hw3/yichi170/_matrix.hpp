#include <vector>
#include <stdexcept>
#include <cstddef>

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
        
        m_buffer = other.m_buffer;
        return *this;
    }

    ~Matrix() {  delete[] m_buffer; }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    
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

Matrix multiply_naive(const Matrix &, const Matrix &);
Matrix multiply_tile(const Matrix &, const Matrix &, size_t);
Matrix multiply_ntile(const Matrix &, const Matrix &, size_t);
Matrix multiply_mkl(const Matrix &, const Matrix &);

