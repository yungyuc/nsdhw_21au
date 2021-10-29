#include "_matrix.hpp"
#include "StopWatch.hpp"
#include <iostream>
#include <algorithm>
#include <mkl.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
namespace py = pybind11;

bool operator==(Matrix const &lhs, Matrix const &rhs){
    if((lhs.m_nrow != rhs.m_nrow) && (lhs.m_ncol != rhs.m_ncol)) { return false; }

    for(size_t i=0; i<lhs.m_ncol; i++){
        for(size_t j=0; j<lhs.m_nrow; j++){
            if(lhs(i, j) != rhs(i, j)) { return false; }
        }
    }
    return true;
}

bool operator!=(Matrix const &lhs, Matrix const &rhs){
    return !(lhs == rhs);
}

void initialize(Matrix &m1){
    for(size_t i=0; i<m1.nrow(); i++){
        for(size_t j=0; j<m1.ncol(); j++){
            m1(i, j) = i*m1.nrow() + j + 1;
        }
    }
}

Matrix multiply_naive(const Matrix &m1, const Matrix &m2){
    const size_t m1nrow  = m1.nrow();
    const size_t m2ncol  = m2.ncol();
    Matrix m3(m1nrow, m2ncol);
    StopWatch sw;
    for(size_t i=0; i<m1nrow; i++){
        for(size_t j=0; j<m2ncol; j++){
            double t = 0;
            for(size_t k=0; k<m1nrow; k++){
                t += m1(i, k) * m2(k, j);
            }
            m3(i, j) = t;
        }
    }
    m3.elapsed() = sw.lap();
    return m3;
}

Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t blocksize){
    const size_t m1nrow = m1.nrow();
    const size_t m1ncol = m1.ncol();
    const size_t m2nrow = m2.nrow();
    const size_t m2ncol  = m2.ncol();
    Matrix m3(m1nrow, m2ncol);
    StopWatch sw;
    for(size_t i=0; i<m1nrow; i+=blocksize){
        for(size_t j=0; j<m1nrow; j+=blocksize){
            m3.m_buffer[i*m3.nrow()+j] = 0.0;
            for (size_t k=0; k<m1ncol; k+=blocksize){
                for(size_t ii=i; ii<std::min(i+blocksize, m1nrow); ii++){
                    for(size_t jj=j; jj<std::min(j+blocksize, m2ncol); jj++){
                        for(size_t kk=k; kk<std::min(k+blocksize, m1ncol); kk++){
                            m3.m_buffer[ii*m3.nrow()+jj] += m1.m_buffer[ii*m1nrow+kk] * m2.m_buffer[kk*m2nrow+jj];
                        }
                        
                    }
                }
            }
        }
    }
    m3.elapsed() = sw.lap();
    return m3;
}

Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());

    StopWatch sw;

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
      , ret.m_buffer /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    );

    ret.elapsed() = sw.lap();

    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::self == py::self)
        .def("__getitem__", &Matrix::getitem)
        .def("__setitem__", &Matrix::setitem)
        .def_readwrite("nrow", &Matrix::m_nrow)
        .def_readwrite("ncol", &Matrix::m_ncol);
    m.def("multiply_mkl", &multiply_mkl, "");
    m.def("multiply_tile", &multiply_tile, "");
    m.def("multiply_naive", &multiply_naive, "");
}

// int main(){
//     size_t S = 1000;
//     size_t blocksize =50;
//     Matrix m1(S, S); 
//     initialize(m1);
//     Matrix m2(S, S);
//     initialize(m2);
//     Matrix m3 = multiply_naive(m1, m2);
//     Matrix m4 = multiply_tile(m1, m2, blocksize);
//     Matrix m5 = multiply_mkl(m1, m2);
//     // m3.print();
//     // m1.print();
//     // std::cout << std::endl;
//     // m2.print();
//     // std::cout << std::endl;
//     // m4.print();
//     // std::cout << std::endl;
//     // m5.print();
//     // std::cout << std::endl;
//     std::cout << "Native: " << m3.elapsed() << "  " << m3.elapsed() / m3.elapsed() << std::endl;
//     std::cout << "Tile: " << m4.elapsed() << "  " << m3.elapsed() / m4.elapsed() << std::endl;
//     std::cout << "mkl: " << m5.elapsed() << "  " << m3.elapsed() / m5.elapsed() << std::endl;
//     if(m3 == m4) std::cout << "Result correct" << std::endl;

//     return 0;
// }
