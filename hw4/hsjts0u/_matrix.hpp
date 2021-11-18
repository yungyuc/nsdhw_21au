#include <cstring>
#include <cstddef>
#include <algorithm>
#include <vector>
#include <atomic>
#include <limits>
#include <mkl.h>


class Matrix 
{

public:
    
    Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer.resize(nelement);
    }
    
    ~Matrix() = default;

    Matrix(const Matrix &M)
    {
        m_nrow = M.m_nrow;
        m_ncol = M.m_ncol;
        m_buffer = M.m_buffer;
    }
    
    Matrix& operator= (const Matrix& M) 
    {
        m_buffer = M.m_buffer;
        return *this;
    }

    bool operator== (Matrix const &M) const 
    {
        for (size_t i = 0; i < m_nrow; i++) 
            for (size_t j = 0;j < m_ncol; j++)
                if (m_buffer[i*m_ncol + j] != M(i, j)) return false;
                
        return true;
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[row*m_ncol + col]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[row*m_ncol + col]; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    
    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> m_buffer;

};

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);
