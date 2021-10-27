#include <pybind11/pybind11.h>

#include "matrix.hpp"
#include "matrix_mul.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "Matrix utilities";

  py::class_<Matrix>(m, "Matrix")
      .def(py::init<>())
      .def(py::init<size_t, size_t>())
      .def_property_readonly("nrow", &Matrix::nrow, "Number of rows")
      .def_property_readonly("ncol", &Matrix::ncol, "Number of columns")
      .def("__getitem__", &Matrix::get_elem)
      .def("__setitem__", &Matrix::set_elem)
      .def("__eq__", &Matrix::operator==)
      .def("__ne__", &Matrix::operator!=)
      .def("__repr__", &Matrix::repr);

  m.def("multiply_naive", &multiply_naive,
        "Multiply two matrices using the naive method", "a"_a, "b"_a,
        py::pos_only());
  m.def("multiply_tile", &multiply_naive,
        "Multiply two matrices using the tiling method", "a"_a, "b"_a,
        py::pos_only());
  m.def("multiply_mkl", &multiply_naive, "Multiply two matrices using MKL",
        "a"_a, "b"_a, py::pos_only());
}
