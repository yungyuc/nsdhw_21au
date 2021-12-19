#include "matrix.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <pybind11/pybind11.h>
#include <cmath>
#include "mkl.h"
#include <stdexcept>

#define EPS 1e-6

namespace py = pybind11;

void Matrix::resize(size_t r, size_t c) {
  if (this->row != r || this->col != c) {
    delete [] mat;
    mat = new double[r * c];
    this->row = r;
    this->col = c;
  }
}

Matrix::Matrix(const Matrix& m) {
  this->row = m.nrow();
  this->col = m.ncol();
  size_t length = m.nrow() * m.ncol();
  this->mat = new double[length];
  for (size_t i = 0; i < length; i++) {
    this->mat[i] = m.mat[i];
  }
}

Matrix::Matrix(size_t r, size_t c) {
  if (r == 0 || c == 0) {
    throw std::out_of_range("matrix size cannot be 0");
  }
  mat = new double[r * c];
  for (size_t i = 0; i < r; i++) {
    for (size_t j = 0; j < c; j++) {
      mat[i*c + j] = 0.0;
    }
  }
  this->row = r;
  this->col = c;
}

Matrix::~Matrix() {
  if (mat != nullptr) delete [] mat;
}

Matrix& Matrix::operator=(const Matrix &m) {
  if (this == &m) { return *this; }
  resize(m.nrow(), m.ncol());
  size_t length = m.nrow() * m.ncol();
  for (size_t i = 0; i < length; i++) {
    this->mat[i] = m.mat[i];
  }
  return (*this);
}

bool Matrix::operator== (const Matrix &B) const {
  if (this->row != B.row || this->col != B.col) return false;
  for (size_t i = 0; i < this->row; i++) {
    for (size_t j = 0; j < this->col; j++) {
      if (abs((*this)(i, j) - B(i, j)) > EPS)
	return false;
    }
  }
  return true;
}

Matrix multiply_naive(const Matrix& A, const Matrix& B) {
  if (A.ncol() != B.nrow()) {
    throw py::value_error("The two matrices cannot be multiplied");
  }
  Matrix C(A.nrow(), B.ncol());
  for (size_t i = 0; i < A.nrow(); i++) {
    for (size_t j = 0; j < B.ncol(); j++) {
      double sum = 0.0;
      for (size_t k = 0; k < A.ncol(); k++) {
	sum += A(i, k) * B(k, j); 
      }
      C(i, j) = sum;
    }
  }
  return C;
}


Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_width) {
  if (A.ncol() != B.nrow()) {
    throw py::value_error("The two matrices cannot be multiplied");
  }
  Matrix C(A.nrow(), B.ncol());

  for (size_t i = 0; i < A.nrow(); i += tile_width) {
    for (size_t j = 0; j < B.ncol(); j += tile_width) {
      size_t rowb = std::min(A.nrow(), i + tile_width);
      size_t colb = std::min(B.ncol(), j + tile_width);
      for (size_t k = 0; k < A.ncol(); k += tile_width) {
        size_t inb = std::min(A.ncol(), k + tile_width);
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
  return C;
}

Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
  if (A.ncol() != B.nrow()) {
    throw py::value_error("The two matrices can not be multiplied");
  }
  Matrix C(A.nrow(), B.ncol());
  int m = A.nrow();
  int k = A.ncol();
  int n = B.ncol();
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
              m, n, k, 1.0, A.mat, k, B.mat, n, 0.0, C.mat, n);
  return C;
}
