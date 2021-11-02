#include <cstring>
#include <cstddef>
#include <mkl.h>

class Matrix 
{
    public:

        Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
        {
            size_t nelement = nrow * ncol;
            m_buffer = new double[nelement];
        }

        ~Matrix()
        {
            delete[] m_buffer;
        }

        Matrix(const Matrix &matrix)
        {
            m_nrow = matrix.m_nrow;
            m_ncol = matrix.m_ncol;
            m_buffer = new double[m_nrow * m_ncol](); 
            memcpy(m_buffer, matrix.m_buffer, m_nrow * m_ncol*sizeof(double));
        }

        bool operator== (Matrix const &matrix) const 
        {
            for (size_t i = 0; i < m_nrow; i++) 
                for (size_t j = 0; j < m_ncol; j++)
                    if (m_buffer[i*m_ncol + j] != matrix(i, j)) return false;

            return true;
        }

        Matrix& operator= (const Matrix& matrix) 
        {
            m_buffer = matrix.m_buffer;
            return *this;
        }


        // No bound check.
        double   operator() (size_t row, size_t col) const
        {
            return m_buffer[row*m_ncol + col];
        }
        double & operator() (size_t row, size_t col)
        {
            return m_buffer[row*m_ncol + col];
        }


        double * get_matrix_buffer() const { return m_buffer; }
        size_t nrow() const { return m_nrow; }
        size_t ncol() const { return m_ncol; }

    private:

        size_t m_nrow;
        size_t m_ncol;
        double * m_buffer;

};



Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2); 