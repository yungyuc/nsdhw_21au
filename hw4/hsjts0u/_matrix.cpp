#include "_matrix.hpp"
#include <algorithm>
#include <cstddef>


static MyAllocator<double> alloc;


Matrix::Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol), m_buffer(alloc)
{
    reset(nrow, ncol);
}


Matrix::Matrix(const Matrix &M)
{
    m_nrow = M.m_nrow;
    m_ncol = M.m_ncol;
    reset(m_nrow, m_ncol);
    
    for (size_t i = 0; i < m_nrow; i++)
        for (size_t j = 0; j < m_ncol; j++)
            (*this)(i, j) = M(i, j);
}


Matrix::Matrix(Matrix && M) 
: m_nrow(std::exchange(M.m_nrow, 0))
, m_ncol(std::exchange(M.m_ncol, 0))
, m_buffer(std::move(M.m_buffer)) {}


Matrix& Matrix::operator= (const Matrix& M) 
{
    if (this == &M) { return *this; }

    if (m_nrow != M.m_nrow || m_ncol != M.m_ncol) reset(M.m_nrow, M.m_ncol);
    for (size_t i = 0; i < m_nrow; i++)
        for (size_t j = 0; j < m_ncol; j++)
            (*this)(i, j) = M(i, j);

    return *this;

}


bool Matrix::operator== (Matrix const &M) const 
{
    for (size_t i = 0; i < m_nrow; i++) 
        for (size_t j = 0;j < m_ncol; j++)
            if (m_buffer[i*m_ncol + j] != M(i, j)) return false;
            
    return true;
}


double   Matrix::operator() (size_t row, size_t col) const { return m_buffer[row*m_ncol + col]; }
double & Matrix::operator() (size_t row, size_t col)       { return m_buffer[row*m_ncol + col]; }
size_t   Matrix::nrow() const { return m_nrow; }
size_t   Matrix::ncol() const { return m_ncol; }


void Matrix::reset(size_t nrow, size_t ncol) 
{
    size_t nelement = nrow * ncol;
    m_buffer.resize(nelement, 0);
    m_nrow = nrow;
    m_ncol = ncol;
}


Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    
    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < ret.nrow(); i++) 
    {
        for (size_t j = 0; j < ret.ncol(); j++) 
        {
            double e = 0;
            for (size_t k = 0; k < mat1.ncol(); k++) {
                e += mat1(i, k) * mat2(k, j);
            }
            ret(i, j) = e;
        }
    }

    return ret;
     
}


Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize)
{
    
    Matrix ret(mat1.nrow(), mat2.ncol());
    
    for (size_t row = 0; row < mat1.nrow(); row += tsize) 
    {
        for (size_t col = 0; col < mat2.ncol(); col += tsize) 
        {
            for (size_t inner = 0; inner < mat1.ncol(); inner += tsize) 
            {
                // operation for tile
                for (size_t k = inner; k < std::min(mat1.ncol(), inner + tsize); k++) 
                {
                    for (size_t i = row; i < std::min(mat1.nrow(), row + tsize); i++) 
                    {
                        for (size_t j = col; j < std::min(mat2.ncol(), col + tsize); j++) 
                        {
                            ret(i, j) += mat1(i, k) * mat2(k, j);
                        }
                    }
                }
            }
        }
    }

    return ret;

}


Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
    
    double alpha = 1.0;
    double beta = 0.0;

    Matrix ret = Matrix(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        mat1.nrow(),
        mat2.ncol(),
        mat1.ncol(),
        alpha,
        mat1.m_buffer.data(),
        mat1.ncol(),
        mat2.m_buffer.data(),
        mat2.ncol(),
        beta,
        ret.m_buffer.data(),
        ret.ncol()
    );

    return ret;

}


size_t bytes() { return alloc.bytes(); }
size_t allocated() { return alloc.allocated(); }
size_t deallocated() { return alloc.deallocated(); }