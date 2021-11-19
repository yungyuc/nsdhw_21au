#ifndef MATRIX_H_
#define MATRIX_H_

#include <cstddef>
#include <vector>
#include "CustomAllocator.h"

static CustomAllocator<double> custom_allocator;

class Matrix {
 public:
  Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(custom_allocator) 
      {
        reset_buffer(nrow, ncol);
      }
  Matrix(const std::vector<std::vector<double>> &m);

  bool operator==(const Matrix &other);
  double operator()(size_t row, size_t col) const {
    return m_buffer[row * m_ncol + col];
  }
  double &operator()(size_t row, size_t col) {
    return m_buffer[row * m_ncol + col];
  }

  size_t nrow() const { return m_nrow; }
  size_t ncol() const { return m_ncol; }
  double *buffer() { return m_buffer.data(); }
  const double *buffer() const { return m_buffer.data(); }

 private:
  size_t index(size_t row, size_t col) const
  {
    return row + col * m_nrow;
  }
  
  void reset_buffer(size_t nrow, size_t ncol)
  {
    m_buffer.resize(nrow * ncol, 0.0);
    
    m_nrow = nrow;
    m_ncol = ncol;
  }
  
  size_t m_nrow;
  size_t m_ncol;
  std::vector<double, CustomAllocator<double>> m_buffer;
};

Matrix multiply_naive(const Matrix &m1, const Matrix &m2);
Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t tsize);
Matrix multiply_mkl(const Matrix &m1, const Matrix &m2);

#endif  // MATRIX_H_
