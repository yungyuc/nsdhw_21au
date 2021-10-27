#include "Mat.hpp"

Matrix::Matrix(Matrix &other) {
  this->m_row = other.m_row;
  this->m_col = other.m_col;
  for (size_t i = 0; i < m_row; ++i) {
    for (size_t j = 0; j < m_col; ++j) {
      this->getpos(i, j) = other.getdata(i, j);
    }
  }
}

Matrix &Matrix::operator=(Matrix &&other) {

  std::swap(m_row, other.m_row);
  std::swap(m_col, other.m_col);
  std::swap(data, other.data);
  return *this;
}

Matrix multiply_naive(Matrix &A_mat, Matrix &B_mat) {
  Matrix res(A_mat.getrow(), B_mat.getcol());
  for (size_t i = 0; i < A_mat.getrow(); ++i) {
    for (size_t j = 0; j < B_mat.getcol(); ++j) {
      double result = 0;
      for (size_t k = 0; k < A_mat.getcol(); ++k) {
        result += A_mat.getdata(i, k) * B_mat.getdata(k, j);
      }
      res.getpos(i, j) = result;
    }
  }
  return res;
}
