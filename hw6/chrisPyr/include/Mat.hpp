#ifndef MAT_HPP_
#define MAT_HPP_

#include <mkl.h>

#include <functional>
#include <iomanip>
#include <iostream>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace py = pybind11;

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

  ~Matrix(){};

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
  double &operator()(size_t row, size_t col) { return data[row * m_col + col]; }

  py::array_t<double> array();
  friend Matrix multiply_naive(Matrix &A_mat, Matrix &B_mat);
  friend Matrix multiply_tile(Matrix &A_mat, Matrix &B_mat, size_t tile_size);
  friend Matrix multiply_mkl(Matrix const &A_mat, Matrix const &B_mat);
};
#endif
