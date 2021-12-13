// pybind11 start
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "include/_GEMM.hpp"
namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "test naive GEMM";

  m.def("multiply_naive", &multiply_naive, "A function which multiply two matrix with naive method.");
  m.def("multiply_tile", &multiply_tile, "A function which multiply two matrix with tile method.");
  m.def("multiply_mkl", &multiply_mkl, "A function which multiply two matrix with mlk method.");
  m.def("bytes", &bytes);
  m.def("allocated", &allocated);
  m.def("deallocated", &deallocated);
  py::class_<Matrix>(m, "Matrix")
    .def(py::init<size_t, size_t>())
    .def(py::init<size_t, size_t, const std::vector<double> &>())
    .def(py::init<const Matrix &>())
    .def_property_readonly("nrow", &Matrix::nrow)
    .def_property_readonly("ncol", &Matrix::ncol)
    .def("__eq__", &operator==)
    .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> idx, double val) { return self(idx.first, idx.second) = val; })
    .def("__getitem__", [](const Matrix &self, std::pair<size_t, size_t> idx) { return self(idx.first, idx.second); });
}
