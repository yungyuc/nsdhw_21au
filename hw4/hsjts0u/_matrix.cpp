#include "_matrix.hpp"
#include <algorithm>
#include <cstddef>


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