#include "_matrix.h"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <cmath>
#include "mkl.h"

#define EPS 1e-6

namespace py = pybind11;

void Matrix::init(size_t row, size_t col) {
  if (mat != nullptr) delete [] mat;
  size_t entry_num = row * col;
  if (entry_num > 0) {
    mat = new double[entry_num];
  }
  for (size_t i = 0; i < row; i++) {
    for (size_t j = 0; j < col; j++) {
      mat[i*col + j] = 0.0;
    }
  }
  this->nrow = row;
  this->ncol = col;
}

Matrix::Matrix(const Matrix& m) {
  this->mat = nullptr;
  init(m.nrow, m.ncol);
  size_t length = m.nrow * m.ncol;
  for (size_t i = 0; i < length; i++) {
    this->mat[i] = m.mat[i];
  }
}

Matrix::~Matrix() {
  if (mat != nullptr) delete [] mat;
}

bool Matrix::operator== (const Matrix &B) const {
  if (this->nrow != B.nrow || this->ncol != B.ncol) return false;
  for (size_t i = 0; i < this->nrow; i++) {
    for (size_t j = 0; j < this->ncol; j++) {
      if (abs((*this)(i, j) - B(i, j)) > EPS)
        return false;
      // if ((*this)(i,j) != B(i,j)) return false;
    }
  }
  return true;
}

std::string Matrix::to_string() const {
  std::stringstream ss;
  ss << "[";
  for (size_t i = 0; i < this->nrow; i++) {
    for (size_t j = 0; j < this->ncol; j++) {
      ss << " " <<(*this)(i, j);
    }
    if (i == this->nrow-1) {
      ss << " ]";
    }
    else ss << std::endl;
  }
  return ss.str();
}


Matrix multiply_naive(const Matrix& A, const Matrix& B) {
  if (A.ncol != B.nrow) {
    throw py::value_error("The two matrices cannot be multiplied");
  }
  Matrix C(A.nrow, B.ncol);
  for (size_t i = 0; i < A.nrow; i++) {
    for (size_t j = 0; j < B.ncol; j++) {
      double sum = 0.0;
      for (size_t k = 0; k < A.ncol; k++) {
	sum += A(i, k) * B(k, j); 
      }
      C(i, j) = sum;
    }
  }
  return C;
}


Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_width) {
  if (A.ncol != B.nrow) {
    throw py::value_error("The two matrices cannot be multiplied");
  }
  Matrix C(A.nrow, B.ncol);
  // double *buf1 = new double[tile_width * tile_width];
  // double *buf2 = new double[tile_width * tile_width];
  // double *buf3 = new double[tile_width * tile_width];

  for (size_t i = 0; i < A.nrow; i += tile_width) {
    for (size_t j = 0; j < B.ncol; j += tile_width) {
      size_t rowb = std::min(A.nrow, i + tile_width);
      size_t colb = std::min(B.ncol, j + tile_width);
      for (size_t k = 0; k < A.ncol; k += tile_width) {
        size_t inb = std::min(A.ncol, k + tile_width);
        for (size_t kk = k; kk < inb; kk++) {
          for (size_t ii = i; ii < rowb; ii++) {
            for (size_t jj = j; jj < colb; jj++) {
              C(ii, jj) += A(ii, kk) * B(kk, jj);
            }
          }
        }
      }
    }
  }

  // delete [] buf1;
  // delete [] buf2;
  // delete [] buf3;
  
  return C;
}

Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
  if (A.ncol != B.nrow) {
    throw py::value_error("The two matrices can not be multiplied");
  }
  Matrix C(A.nrow, B.ncol);
  int m = A.nrow;
  int k = A.ncol;
  int n = B.ncol;
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
              m, n, k, 1.0, A.mat, k, B.mat, n, 0.0, C.mat, n);
  return C;
}

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "a matrix multipicate library with three different multiplication methods";
  m.def("multiply_tile", &multiply_tile, "Multiply two matrices by tiling");
  m.def("multiply_naive", &multiply_naive, "Multiply two matrices with the naive method");
  m.def("multiply_mkl", &multiply_mkl, "Multiply two matrices with mkl");

  py::class_<Matrix>(m, "Matrix")
    .def(py::init<size_t, size_t>())
    .def("__getitem__",
         [](const Matrix &mat, std::array<int, 2> idx) {
           return mat(idx[0], idx[1]);
         })
    .def("__setitem__",
         [](Matrix &mat, std::array<int, 2> idx, double val) {
           mat(idx[0], idx[1]) = val;
         })
    .def(py::self == py::self)
    .def("__str__",
         [](Matrix &mat) {
           return mat.to_string();
         })
    .def_readonly("nrow", &Matrix::nrow)
    .def_readonly("ncol", &Matrix::ncol);
}

