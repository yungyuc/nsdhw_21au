#include "_matrix.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
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

Matrix multiply_tile(Matrix const & a, Matrix const & b, size_t tile)
{
    if (a.ncol() != b.nrow()) {
        throw out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
    size_t bound_i, bound_j, bound_k;
    Matrix ans(a.nrow(), b.ncol());

    for (size_t i = 0; i < a.nrow(); i += tile)
    {
        for (size_t j = 0; j < b.ncol(); j += tile) 
        {
            bound_i = std::min(a.nrow(), i + tile);
            bound_j = std::min(b.ncol(), j + tile);
            for (size_t k = 0; k < a.ncol(); k += tile)
            {
                bound_k = std::min(a.ncol(), k + tile);

                for (size_t t_k = k; t_k < bound_k; t_k++)
                {
                    for (size_t t_i = i; t_i < bound_i; t_i++)
                    {
                        for (size_t t_j = j; t_j < bound_j; t_j++)
                        {
                            ans(t_i, t_j) += a(t_i, t_k) * b(t_k, t_j);
                        }
                    }
                }
            }
        }
    }
    return ans;

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