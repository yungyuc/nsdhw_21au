#include "_matrix.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstddef>
#include <algorithm>

namespace py = pybind11;
using namespace std;

Matrix multiply_naive(Matrix const & matrix1, Matrix const & matrix2)
{

    Matrix ret(matrix1.nrow(), matrix2.ncol());

    for (size_t row = 0; row < ret.nrow(); row++) 
    {
        for (size_t column = 0; column < ret.ncol(); column++) 
        {
            double value = 0;
            for (size_t k = 0; k < matrix1.ncol(); k++) {
                value += matrix1(row, k) * matrix2(k, column);
            }
            ret(row, column) = value;
        }
    }

    return ret;

}

Matrix multiply_tile(Matrix const & matrix1, Matrix const & matrix2, size_t tilesize)
{

    Matrix ret(matrix1.nrow(), matrix2.ncol());

    for (size_t row = 0; row < matrix1.nrow(); row += tilesize) 
    {
        for (size_t col = 0; col < matrix2.ncol(); col += tilesize) 
        {
            for (size_t inner = 0; inner < matrix1.ncol(); inner += tilesize) 
            {


                for (size_t k = inner; k < std::min(matrix1.ncol(), inner + tilesize); k++) 
                {
                    for (size_t i = row; i < std::min(matrix1.nrow(), row + tilesize); i++) 
                    {
                        for (size_t j = col; j < std::min(matrix2.ncol(), col + tilesize); j++)
                        {
                            ret(i, j) += matrix1(i, k) * matrix2(k, j);
                        }
                    }
                }


                
            }
        }
    }

    return ret;

}

Matrix multiply_mkl(Matrix const & matrix1, Matrix const & matrix2)
{

    Matrix ret = Matrix(matrix1.nrow(), matrix2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        matrix1.nrow(),
        matrix2.ncol(),
        matrix1.ncol(),
        1.0,
        matrix1.get_matrix_buffer(),
        matrix1.ncol(),
        matrix2.get_matrix_buffer(),
        matrix2.ncol(),
        0.0,
        ret.get_matrix_buffer(),
        ret.ncol()
    );

    return ret;
} 


PYBIND11_MODULE(_matrix, m) {
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);

  py::class_<Matrix>(m, "Matrix")
    .def(py::init<size_t, size_t>())
    //.def(py::init<const Matrix &matrix> &>())
    .def_property_readonly("nrow", &Matrix::nrow)
    .def_property_readonly("ncol", &Matrix::ncol)

    .def("__eq__", [](const Matrix &self, const Matrix &other) { return self == other; })
    .def("assign", &Matrix::operator=)
    .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> idx, double val) { return self(idx.first, idx.second) = val; })
    .def("__getitem__", [](const Matrix &self, std::pair<size_t, size_t> idx) { return self(idx.first, idx.second); });
}