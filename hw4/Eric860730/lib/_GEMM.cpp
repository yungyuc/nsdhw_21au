#include <iostream>
#include <stdexcept>

#include <mkl.h>

#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <atomic>
#include <vector>

#include "../include/_GEMM.hpp"

void validate_multiplication(Matrix const &mat1, Matrix const &mat2) {
  if (mat1.ncol() != mat2.nrow()) {
    throw std::out_of_range(
        "the number of first matrix column "
        "differs from that of second matrix row");
  }
}

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2) {
  validate_multiplication(mat1, mat2);
  Matrix m_result(mat1.nrow(), mat2.ncol());

  // row-major
  for (size_t i = 0; i < mat1.nrow(); ++i) {
    for (size_t j = 0; j < mat2.ncol(); ++j) {
      double v = 0;
      for (size_t k = 0; k < mat2.nrow(); ++k) {
        v += mat1(i, k) * mat2(k, j);
      }
      m_result(i, j) = v;
      //printf("m_result(%d,%d) = {%f}", i, j, m_result(i,j));
    }
  }

  return m_result;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tile_size) {
  validate_multiplication(mat1, mat2);
  const size_t m1_row = mat1.nrow();
  const size_t m2_col = mat2.ncol();
  const size_t m2_row = mat2.nrow();

  Matrix m_result(m1_row, m2_col);
  for (size_t i = 0; i < m1_row; ++i) {
    for (size_t j = 0; j < m2_col; ++j) {
      m_result(i, j) = 0;
    }
  }

  for (size_t i = 0; i < m1_row; i+=tile_size) {
    for (size_t j = 0; j < m2_col; j+=tile_size) {
      for (size_t k = 0; k < m2_row; k+=tile_size) {
        // tile(i, j) = tile(j, k) * tile(k, i)
        // calculate tile(i, j) with inner loop.
        for (size_t ti = i; ti < std::min(i + tile_size, m1_row); ++ti) {
          for (size_t tj = j; tj < std::min(j + tile_size, m2_col); ++tj) {
            double val = 0;
            for (size_t tk = k; tk < std::min(k + tile_size, m2_row); ++tk) {
              val += mat1(ti, tk) * mat2(tk, tj);
            }
            m_result(ti, tj) += val;
          }
        }
      }
    }
  }

  return m_result;

}

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
  validate_multiplication(mat1, mat2);

  mkl_set_num_threads(1);

  Matrix ret(mat1.nrow(), mat2.ncol());

  cblas_dgemm(
      CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.m_buffer.data() /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.m_buffer.data() /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.m_buffer.data() /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
      );

  return ret;
}

bool operator==(Matrix const &mat1, Matrix const &mat2) {
  if ((mat1.ncol() != mat2.ncol()) && (mat1.nrow() != mat2.ncol())) {
    return false;
  }

  for (size_t i = 0; i < mat1.nrow(); ++i) {
    for (size_t j = 0; j < mat1.ncol(); ++j) {
      if (mat1(i, j) != mat2(i, j)) {
        return false;
      }
    }
  }

  return true;
}

std::size_t bytes() { return matrix_allocator.counter.bytes(); }
std::size_t allocated() { return matrix_allocator.counter.allocated(); }
std::size_t deallocated() { return matrix_allocator.counter.deallocated(); }

// pybind11 start
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

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
