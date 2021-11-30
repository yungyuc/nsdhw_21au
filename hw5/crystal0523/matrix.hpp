#include <stdexcept>
#include <mkl.h>

class Matrix {

public:

    Matrix(const size_t nrow, const size_t ncol): nrow(nrow), ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }
    ~Matrix()
    {
        reset_buffer(0,0);
    }

    bool operator== (Matrix const & mat1)const;
    double   operator() (size_t row, size_t col) const { return buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col) { return buffer[index(row, col)]; }
    size_t index(size_t row, size_t col) const{ return row * ncol + col; }
    void reset_buffer(size_t row, size_t col)
    {
        if (buffer) { delete[] buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { buffer = new double[nelement]; }
        else          { buffer = nullptr; }
        nrow = row;
        ncol = col;
        
    }
    
    size_t nrow = 0;
    size_t ncol = 0;
    double * buffer = nullptr;
    
};

   
bool Matrix::operator== (Matrix const & mat1)const
{
    if (((*this).ncol != mat1.ncol) || ((*this).nrow != mat1.nrow))return false;
    
    for (size_t i=0; i< nrow; i++)
    {
        for (size_t j=0; j< ncol; j++)
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
    if (mat1.ncol != mat2.nrow)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix mat3(mat1.nrow, mat2.ncol);

    for (size_t i=0; i<mat3.nrow; i++)
    {
        for (size_t j=0; j<mat3.ncol; j++)
        {
            double v = 0;
            for (size_t k=0; k<mat1.ncol; k++)
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

Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t size)
{
    if (mat1.ncol != mat2.nrow)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

   
    Matrix mat3(mat1.nrow, mat2.ncol);

    for (size_t it=0; it<mat1.nrow; it+=size)
    {
        for (size_t kt=0; kt<mat2.ncol; kt+=size)
        {
            for (size_t jt=0; jt<mat1.ncol; jt+=size)
            {
                 for (size_t k = jt; k < std::min(mat1.ncol, jt + size); k++) 
                {
                    for (size_t i = it; i < std::min(mat1.nrow, it + size); i++) 
                    {
                        for (size_t j =kt; j < std::min(mat2.ncol, kt + size); j++) {
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
    
    Matrix mat3(mat1.nrow, mat2.ncol);

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow /* const MKL_INT m */
      , mat2.ncol /* const MKL_INT n */
      , mat1.ncol /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.buffer /* const double *a */
      , mat1.ncol /* const MKL_INT lda */
      , mat2.buffer /* const double *b */
      , mat2.ncol /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , mat3.buffer /* double * c */
      , mat3.ncol /* const MKL_INT ldc */
    );

    return mat3;
}
