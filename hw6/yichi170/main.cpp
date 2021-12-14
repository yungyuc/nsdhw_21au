#include <chrono>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include "matrix.hpp"

#define fs first
#define sc second

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<const size_t, const size_t>())
        .def_readwrite("nrow", &Matrix::m_nrow)
        .def_readwrite("ncol", &Matrix::m_ncol)
        .def("assign", &Matrix::operator=)
        .def("__setitem__", 
            [](Matrix &mt, std::pair<size_t, size_t> idx, double val) {
                return mt(idx.first, idx.second) = val;
            })
        .def("__getitem__", [](Matrix &mt, std::pair<size_t, size_t>idx) {
                return mt(idx.first, idx.second);
            })
        .def("__eq__", [](const Matrix &mt1, const Matrix &mt2) {
                return mt1 == mt2;
            })
        .def_buffer([](Matrix &m) -> py::buffer_info {
            return py::buffer_info(
                m.data(), 
                sizeof(double), 
                py::format_descriptor<double>::format(), 
                2, 
                { m.ncol(), m.ncol() }, 
                { sizeof(double) * m.ncol(), sizeof(double) }
            );
        });
        
}
