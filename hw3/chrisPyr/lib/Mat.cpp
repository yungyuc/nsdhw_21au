#include "Mat.hpp"
#include <iostream>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

Matrix &Matrix::operator=(const Matrix &other) {
  data = other.data;
  return *this;
}

bool Matrix::operator==(const Matrix &other) const {
  for (size_t i = 0; i < m_row; ++i) {
    for (size_t j = 0; j < m_col; ++j) {
      if (this->getdata(i, j) != other.getdata(i, j)) {
        return 0;
      }
    }
  }
  return 1;
}

Matrix multiply_naive(Matrix &A_mat, Matrix &B_mat) {
  if (A_mat.col() != B_mat.row()) {
    throw std::runtime_error("dimension not match");
  }

  Matrix res(A_mat.row(), B_mat.col());

  for (size_t i = 0; i < A_mat.row(); ++i) {
    for (size_t j = 0; j < B_mat.col(); ++j) {
      double result = 0;
      for (size_t k = 0; k < A_mat.col(); ++k) {
        result += A_mat.getdata(i, k) * B_mat.getdata(k, j);
      }
      res.getpos(i, j) = result;
    }
  }
  return res;
}

Matrix multiply_tile(Matrix &A_mat, Matrix &B_mat, size_t tile_size) {
  size_t row1 = A_mat.row();
  size_t col = B_mat.col();
  size_t row2 = A_mat.col();
  Matrix res(A_mat.row(), B_mat.col());
  for (size_t i = 0; i < row1; i += tile_size) {
    for (size_t j = 0; j < col; j += tile_size) {
      res.getpos(i, j) = 0.0;
      for (size_t k = 0; k < row2; k += tile_size) {
        for (size_t x = i; x < std::min(i + tile_size, row1); x++) {
          for (size_t y = j; y < std::min(j + tile_size, col); y++) {
            for (size_t z = k; z < std::min(k + tile_size, row2); z++) {

              res.getpos(x, y) += A_mat.getdata(x, z) * B_mat.getdata(z, y);
            }
          }
        }
      }
    }
  }
  return res;
}

Matrix multiply_mkl(Matrix const &A_mat, Matrix const &B_mat) {
  mkl_set_num_threads(1);

  Matrix ret(A_mat.row(), B_mat.col());

  cblas_dgemm(CblasRowMajor /* const CBLAS_LAYOUT Layout */
              ,
              CblasNoTrans /* const CBLAS_TRANSPOSE transa */
              ,
              CblasNoTrans /* const CBLAS_TRANSPOSE transb */
              ,
              A_mat.row() /* const MKL_INT m */
              ,
              B_mat.col() /* const MKL_INT n */
              ,
              A_mat.col() /* const MKL_INT k */
              ,
              1.0 /* const double alpha */
              ,
              A_mat.data /* const double *a */
              ,
              A_mat.col() /* const MKL_INT lda */
              ,
              B_mat.data /* const double *b */
              ,
              B_mat.col() /* const MKL_INT ldb */
              ,
              0.0 /* const double beta */
              ,
              ret.data /* double * c */
              ,
              ret.col() /* const MKL_INT ldc */
  );
  return ret;
}

PYBIND11_MODULE(_matrix, m) {
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);
  py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def_property_readonly("nrow", &Matrix::row)
      .def_property_readonly("ncol", &Matrix::col)
      .def(py::self == py::self)
      .def("__setitem__",
           [](Matrix &mat, std::pair<size_t, size_t> id, double val) {
             mat.getpos(id.first, id.second) = val;
           })
      .def("__getitem__", [](Matrix &mat, std::pair<size_t, size_t> id) {
        return mat.getdata(id.first, id.second);
      });
}
