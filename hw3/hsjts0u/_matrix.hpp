#include <cstddef>
#include <algorithm>
#include <mkl.h>

class Matrix 
{
public:
    
    Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement]();
    }
    
    ~Matrix()
    {
        delete [] m_buffer;
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
    double * get_buffer() const { return m_buffer; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

private:
    
    size_t m_nrow;
    size_t m_ncol;
    double *m_buffer;

};

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize);
Matrix& multiply_mkl(Matrix const & mat1, Matrix const & mat2);