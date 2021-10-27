#ifndef MAT_HPP_
#define MAT_HPP_

#include <iostream>
#include <stddef.h>
#include <utility>
#include <mkl.h>

class Matrix {
private:
  size_t m_row, m_col; double *data;

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

  ~Matrix() { delete data; };

  Matrix(Matrix &other);

  Matrix &operator=(Matrix &&other);

  friend std::ostream &operator<<(std::ostream &os, Matrix &mat) {
    for (size_t i = 0; i < mat.m_row; ++i) {
      for (size_t j = 0; j < mat.m_col; ++j) {
        os << mat.getdata(i, j) << ' ';
      }
      os << std::endl;
    }
    return os;
  }

  size_t row() { return m_row; }
  size_t col() { return m_col; }
  double getdata(size_t row, size_t col) { return data[row * m_col + col]; }
  double &getpos(size_t row, size_t col) { return data[row * m_col + col]; }
  friend Matrix multiply_naive(Matrix &A_mat, Matrix &B_mat);
  friend Matrix multiply_tile(Matrix &A_mat, Matrix &B_mat);
  friend Matrix multiply_mkl(Matrix &A_mat, Matrix &B_mat);
};

#endif
