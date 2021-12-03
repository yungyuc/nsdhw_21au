#ifndef _MATRIX_H
#define _MATRIX_H
#include <cstddef>
#include <string>
#include <vector>

class Matrix {
 public:
  Matrix(): mat(nullptr), row(0), col(0) {}
  Matrix(size_t r, size_t c);
  Matrix(const Matrix &m);
  ~Matrix();

  Matrix &operator= (const Matrix &m);
  
  double& operator() (size_t i, size_t j) {
    return this->mat[i * (this->col) + j];
  }

  double operator() (size_t i, size_t j) const {
    return this->mat[i * (this->col) + j];
  }

  size_t nrow() const { return this->row; }
  size_t ncol() const { return this->col; }
  
  bool operator== (const Matrix& B) const;
  friend Matrix multiply_mkl(const Matrix& A, const Matrix&B);
 private:
  double *mat;
  size_t row, col;
  void resize(size_t nrow, size_t ncol);
};

Matrix multiply_naive(const Matrix& A, const Matrix& B);
Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_width);
Matrix multiply_mkl(const Matrix& A, const Matrix& B);

#endif
