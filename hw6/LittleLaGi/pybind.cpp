#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "matrix.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, const std::vector<double> &>())
        .def(py::init<const Matrix &>())
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def("__eq__", &operator==)
        .def("buffer_vector", &Matrix::buffer_vector)
		.def_property("array", &Matrix::array, nullptr)
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i,
                               double val) { mat(i.first, i.second) = val; })
        .def("__getitem__", [](Matrix &mat, std::pair<size_t, size_t> i) {
            return mat(i.first, i.second);
        });
}