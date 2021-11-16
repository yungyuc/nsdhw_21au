#ifndef MAT_HPP_
#define MAT_HPP_

#include <iostream>
#include <mkl.h>
#include <stddef.h>
#include <utility>
#include "Allocator.hpp"

static MyAllocator<double> Allocator;
class Matrix {
private:
  size_t m_row, m_col;
//  double *data = nullptr;
  std::vector<double,MyAllocator<double>> data;

public:
  Matrix(size_t row, size_t col) : m_row{row}, m_col{col}, data{Allocator} {
    //data = new double[m_row * m_col];
    const size_t nelement = m_row * m_col;
    data.reserve(nelement);
    // data initialize
    for (size_t i = 0; i < row; ++i) {
      for (size_t j = 0; j < col; ++j) {
        data[i * m_col + j] = 0.0f;
      }
    }
  };

  //~Matrix() {
  //  if(data){
  //  delete [] data;}
  //};

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
  friend Matrix multiply_naive(Matrix &A_mat, Matrix &B_mat);
  friend Matrix multiply_tile(Matrix &A_mat, Matrix &B_mat, size_t tile_size);
  friend Matrix multiply_mkl(Matrix const &A_mat, Matrix const &B_mat);
};

#endif
