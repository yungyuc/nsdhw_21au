#include "_matrix.hpp"
#include <chrono>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#define fs first
#define sc second

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    py::class_<Matrix>(m, "Matrix")
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
        });
}