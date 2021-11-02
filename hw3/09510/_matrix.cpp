#include "_matrix.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace pybind = pybind11;
using namespace std;

Matrix multiply_naive(Matrix const & a, Matrix const & b)
{

    Matrix ans(a.nrow(), b.ncol());


    for (size_t i = 0; i < a.nrow(); ++i) {
        for (size_t j = 0; j < b.ncol(); ++j) {
            double v = 0;
            for (size_t k = 0; k < b.nrow(); ++k) {
                v += a(i, k) * b(k, j);
            }
            ans(i, j) = v;
        }

    }

    return ans;

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
                        for (size_t j = col; j < std::min(mat2.ncol(), col + tsize); j++) {
                            ret(i, j) += mat1(i, k) * mat2(k, j);
                        }
                    }
                }
            }
        }
    }

    return ret;

}
}

Matrix multiply_mkl(Matrix const & a, Matrix const & b)
{

    Matrix ans = Matrix(a.nrow(), b.ncol());

    cblas_dgemm(
         CblasRowMajor
        ,CblasNoTrans
        ,CblasNoTrans
        ,a.nrow()
        ,b.ncol()
        ,a.ncol()
        ,1.0
        ,a.get_matrix_buffer()
        ,a.ncol()
        ,b.get_matrix_buffer()
        ,b.ncol()
        ,0.0
        ,ans.get_matrix_buffer()
        ,ans.ncol()
    );

    return ans;
} 


PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix multiply";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    pybind::class_<Matrix>(m, "Matrix")
        .def(pybind::init<int, int>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__eq__", [](const Matrix &mat, const Matrix &other) { return mat == other; })
        .def("assign", &Matrix::operator=)
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> idx, double val) { return mat(idx.first, idx.second) = val; })
        .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> idx) { return mat(idx.first, idx.second); });
} 