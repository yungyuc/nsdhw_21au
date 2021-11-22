#include <iostream>
#include <vector>
#include <stdexcept>
#include "MyAllocator.cpp"
#include <mkl.h>
#include <pybind11/stl.h>
#include<pybind11/pybind11.h>
#include<pybind11/operators.h>

namespace py = pybind11;

static MyAllocator<double> alloc;

class Matrix {

public:

    Matrix(const size_t nrow, const size_t ncol): nrow(nrow), ncol(ncol), buffer(alloc)
    {
        reset_buffer(nrow, ncol);
    }
    ~Matrix()
    {
        reset_buffer(0,0);
    }
    Matrix & operator=(Matrix const & other);  //copy assignment
    //copy constructor
    Matrix(Matrix const &other) : nrow(other.nrow), ncol(other.ncol), buffer(alloc) 
    {
        reset_buffer(other.nrow, other.ncol);
        for (size_t i = 0; i < nrow; i++) 
        {
            for (size_t j = 0; j < ncol; j++) 
            {
                (*this)(i, j) = other(i, j);
            }
        }
    }
    Matrix(Matrix &&other): buffer(alloc) //move constructor
    {
        nrow = other.nrow;
        ncol = other.ncol;
        std::swap(buffer, other.buffer);
    }
    bool operator== (Matrix const & mat1)const;
    double   operator() (size_t row, size_t col) const { return buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col) { return buffer[index(row, col)]; }
    size_t index(size_t row, size_t col) const{ return row * ncol + col; }
    void reset_buffer(size_t row, size_t col)
    {
        const size_t nelement = row * col;
        buffer.reserve(nelement);
        nrow = row;
        ncol = col;
    }
    std::vector<double,  MyAllocator<double>> buffer;
    size_t nrow = 0;
    size_t ncol = 0;
    
};

size_t allocated() { return alloc.counter.allocated(); };
size_t deallocated() { return alloc.counter.deallocated(); };
size_t bytes() { return alloc.counter.bytes(); };


//error
Matrix & Matrix::operator=(Matrix const & other)
{
    if (this == &other) { return *this; }
    if (nrow != other.nrow || ncol != other.ncol)
    {
        reset_buffer(other.nrow, other.ncol);
    }
    for(size_t i = 0; i < other.nrow; i++)
    {
        for(size_t j = 0;j<other.ncol; j++)
        {
            (*this)(i,j) = other(i,j);
        }
    }
    return *this;
}
   
   
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
      , mat1.buffer.data() /* const double *a */
      , mat1.ncol /* const MKL_INT lda */
      , mat2.buffer.data() /* const double *b */
      , mat2.ncol /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , mat3.buffer.data() /* double * c */
      , mat3.ncol /* const MKL_INT ldc */
    );

    return mat3;
}


PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
   
     py::class_<Matrix>(m, "Matrix")
        .def(py::init<const size_t, const size_t>())
        .def_readwrite("nrow", &Matrix::nrow)
        .def_readwrite("ncol", &Matrix::ncol)
        .def("__getitem__", [](Matrix &m, std::pair<size_t, size_t>i){
            return m(i.first, i.second);
        }, py::is_operator())
        .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t>i, const double v){
            return m(i.first, i.second)=v;
        }, py::is_operator())     
        .def("assign", &Matrix::operator=)
        .def(py::self==py::self);
}
