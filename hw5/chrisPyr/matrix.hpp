#ifndef MAT_HPP_
#define MAT_HPP_

#include <mkl.h>

#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

class Matrix {
private:
  size_t m_row, m_col;
  double *data = nullptr;

public:
  Matrix(size_t row, size_t col) : m_row{row}, m_col{col} {
    data = new double[m_row * m_col];
    // data initialize
    for (size_t i = 0; i < row; ++i) {
      for (size_t j = 0; j < col; ++j) {
        data[i * m_col + j] = 0.0f;
      }
    }
  };

  ~Matrix() {
  };

  Matrix &operator=(const Matrix &other);
  bool operator==(const Matrix &other) const;

  friend std::ostream &operator<<(std::ostream &os, Matrix &mat) {
    for (size_t i = 0; i < mat.m_row; ++i) {
      for (size_t j = 0; j < mat.m_col; ++j) {
        os << mat.getdata(i, j) << ' ';
      }
      os << std::endl;
    }
    return os;
  }

  size_t nrow() const { return m_row; }
  size_t ncol() const { return m_col; }
  double getdata(size_t row, size_t col) const {
    return data[row * m_col + col];
  }
  double &getpos(size_t row, size_t col) { return data[row * m_col + col]; }
 double operator()(size_t row, size_t col) const {
        return data[row * m_col + col];
    }
    double &operator()(size_t row, size_t col) {
        return data[row * m_col + col];
    }
  friend Matrix multiply_naive(Matrix &A_mat, Matrix &B_mat);
  friend Matrix multiply_tile(Matrix &A_mat, Matrix &B_mat, size_t tile_size);
  friend Matrix multiply_mkl(Matrix const &A_mat, Matrix const &B_mat);
};

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
  if (A_mat.ncol() != B_mat.nrow()) {
    throw std::runtime_error("dimension not match");
  }

  Matrix res(A_mat.nrow(), B_mat.ncol());

  for (size_t i = 0; i < A_mat.nrow(); ++i) {
    for (size_t j = 0; j < B_mat.ncol(); ++j) {
      double result = 0;
      for (size_t k = 0; k < A_mat.ncol(); ++k) {
        result += A_mat.getdata(i, k) * B_mat.getdata(k, j);
      }
      res.getpos(i, j) = result;
    }
  }
  return res;
}

Matrix multiply_tile(Matrix &A_mat, Matrix &B_mat, size_t tile_size) {
  size_t row1 = A_mat.nrow();
  size_t col = B_mat.ncol();
  size_t row2 = A_mat.ncol();
  Matrix res(A_mat.nrow(), B_mat.ncol());
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

  Matrix ret(A_mat.nrow(), B_mat.ncol());

  cblas_dgemm(CblasRowMajor /* const CBLAS_LAYOUT Layout */
              ,
              CblasNoTrans /* const CBLAS_TRANSPOSE transa */
              ,
              CblasNoTrans /* const CBLAS_TRANSPOSE transb */
              ,
              A_mat.nrow() /* const MKL_INT m */
              ,
              B_mat.ncol() /* const MKL_INT n */
              ,
              A_mat.ncol() /* const MKL_INT k */
              ,
              1.0 /* const double alpha */
              ,
              A_mat.data /* const double *a */
              ,
              A_mat.ncol() /* const MKL_INT lda */
              ,
              B_mat.data /* const double *b */
              ,
              B_mat.ncol() /* const MKL_INT ldb */
              ,
              0.0 /* const double beta */
              ,
              ret.data /* double * c */
              ,
              ret.ncol() /* const MKL_INT ldc */
  );
  return ret;
}
#endif
