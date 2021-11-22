#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include "MyAllocater.cpp"
#include <mkl.h>
#include<pybind11/pybind11.h>
#include<pybind11/operators.h>

namespace py = pybind11;

static MyAllocator<double> alloc;

class Matrix {

public:
   
    std::vector<double,  MyAllocator<double>> buffer_;

    Matrix(size_t nrow, size_t ncol): row_(nrow), col_(ncol), buffer_(alloc)
    {
        buffer_.resize(size());
    }
    ~Matrix()
    {
        reset_buffer(0,0);
    }
    Matrix & operator=(Matrix const & other);
    bool operator== (Matrix const & mat1)const;
    double   operator() (size_t row, size_t col) const { return buffer_[index(row, col)]; }
    double & operator() (size_t row, size_t col) { return buffer_[index(row, col)]; }
    size_t nrow() const { return row_; }
    size_t ncol() const { return col_; }
    size_t size() const { return row_ * col_; }
    size_t index(size_t row, size_t col) const{ return row * col_ + col; }
    void reset_buffer(size_t nrow, size_t ncol)
    {
        const size_t nelement = nrow * ncol;
        buffer_.resize(nelement, 0);
        row_ = nrow;
        col_ = ncol;
    }

private:
    size_t row_ = 0;
    size_t col_ = 0;
    
    
};

size_t allocated() { return alloc.counter.allocated(); };
size_t deallocated() { return alloc.counter.deallocated(); };
size_t bytes() { return alloc.counter.bytes(); };

//error
Matrix & Matrix::operator=(Matrix const & other)
{
    if (this == &other) { return *this; }
    if (row_ != other.row_ || col_ != other.col_)
    {
        reset_buffer(other.row_, other.col_);
    }
    for(int i=0; i<other.row_ ;i++)
    {
        for(int j=0;j<other.col_; j++)
        {
            (*this)(i,j) = other(i,j);
        }
    }
    return *this;
}
   
   
bool Matrix::operator== (Matrix const & mat1)const
{
    if ((col_ != mat1.col_) || (row_ != mat1.row_))return false;
    
    for (size_t i=0; i<(*this).row_; ++i)
    {
        for (size_t j=0; j<(*this).col_; ++j)
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
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix mat3(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<mat3.nrow(); ++i)
    {
        for (size_t k=0; k<mat3.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            mat3(i,k) = v;
        }
    }

    return mat3;
}


/*
 * Tiled matrix matrix multiplication.
 */

Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t size)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

   
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
      , mat1.buffer_.data() /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.buffer_.data() /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , mat3.buffer_.data() /* double * c */
      , mat3.ncol() /* const MKL_INT ldc */
    );

    return mat3;
}



PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
    m.def("bytes", &bytes);
     py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("__getitem__", [](Matrix &m, std::pair<size_t, size_t>i){
            return m(i.first, i.second);
        }, py::is_operator())
        .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t>i, const double v){
            return m(i.first, i.second)=v;
        }, py::is_operator())     
        .def("reset_buffer", &Matrix::reset_buffer)
        .def("assign", &Matrix::operator=)
        .def(py::self==py::self)   
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);

}
