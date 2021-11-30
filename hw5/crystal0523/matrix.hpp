#include <mkl.h>
#include <iostream>
class Matrix {
public:

    Matrix(const size_t nrow, const size_t ncol): m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix & operator=(Matrix const & other)
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
       
        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }
    bool operator== (Matrix const & mat1)const;
    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col) { return m_buffer[index(row, col)]; }
    size_t index(size_t row, size_t col) const{ return row * m_ncol + col; }
    void reset_buffer(size_t row, size_t col)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = row * col;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        m_nrow = row;
        m_ncol = col;
        
    }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;
    
};

bool Matrix::operator== (Matrix const & mat1)const
{
    if ((m_ncol != mat1.m_ncol) || (m_nrow != mat1.m_nrow))return false;
    
    for (size_t i=0; i< m_nrow; i++)
    {
        for (size_t j=0; j< m_ncol; j++)
        {
            if ((*this)(i, j) != mat1(i, j)) return false;
        }
    }

    return true;
}



/*
 * Naive matrix matrix multiplication.
 */

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
   
    Matrix mat3(mat1.m_nrow, mat2.m_ncol);

    for (size_t i=0; i<mat3.m_nrow; i++)
    {
        for (size_t j=0; j<mat3.m_ncol; j++)
        {
            double v = 0;
            for (size_t k=0; k<mat1.m_ncol; k++)
            {
                v += mat1(i,k) * mat2(k,j);
            }
            mat3(i,j) = v;
        }
    }

    return mat3;
}


/*
 * Tiled matrix matrix multiplication.
 */

Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t const size)
{
   
    Matrix mat3(mat1.nrow(), mat2.ncol());

    for (size_t it=0; it<mat1.nrow(); it+=size)
    {
        for (size_t kt=0; kt<mat2.ncol(); kt+=size)
        {
            for (size_t jt=0; jt<mat1.ncol(); jt+=size)
            {
                 for (size_t k = jt; k < std::min(mat1.ncol(), jt + size); k++) 
                {
                    for (size_t i = it; i < std::min(mat1.nrow(), it + size); i++) 
                    {
                        for (size_t j =kt; j < std::min(mat2.ncol(), kt + size); j++) {
                            mat3(i, j) += mat1(i, k) * mat2(k, j);
                        }
                    }
                }
                
            }
            
        }
    }
    return mat3;
}


/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
    mkl_set_num_threads(1);
    
    Matrix mat3(mat1.nrow(), mat2.ncol());

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
      , mat3.m_buffer /* double * c */
      , mat3.ncol() /* const MKL_INT ldc */
    );

    return mat3;
}

