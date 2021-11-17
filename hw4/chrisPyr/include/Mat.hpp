#ifndef MAT_HPP_
#define MAT_HPP_

#include "Allocator.hpp"
#include <iostream>
#include <mkl.h>
#include <stddef.h>
#include <utility>

static MyAllocator<double> Allocator;
class Matrix {
private:
  size_t m_row = 0, m_col = 0;
  //  double *data = nullptr;
  std::vector<double, MyAllocator<double>> data;

public:
  Matrix(size_t row, size_t col) : m_row{row}, m_col{col}, data{Allocator} {
    // data = new double[m_row * m_col];
    resetBuffer(row, col);
  };

  ~Matrix() { resetBuffer(0, 0); };

  Matrix(Matrix &&other)
      : m_row{other.m_row}, m_col{other.m_col}, data{Allocator} {
    resetBuffer(0, 0);
    std::swap(m_row, other.m_row);
    std::swap(m_col, other.m_col);
    std::swap(data, other.data);
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

  size_t row() const { return m_row; }
  size_t col() const { return m_col; }
  double getdata(size_t row, size_t col) const {
    return data[row * m_col + col];
  }
  double &getpos(size_t row, size_t col) { return data[row * m_col + col]; }
  void resetBuffer(size_t row, size_t col) {
    size_t elements = row * col;
    data.resize(elements, 0);
    m_row = row;
    m_col = col;
  }
  friend Matrix multiply_naive(Matrix &A_mat, Matrix &B_mat);
  friend Matrix multiply_tile(Matrix &A_mat, Matrix &B_mat, size_t tile_size);
  friend Matrix multiply_mkl(Matrix const &A_mat, Matrix const &B_mat);
};

#endif
