#ifndef _MATRIX_H
#define _MATRIX_H
#include <cstddef>
#include <string>
#include <vector>

class Matrix {
 public:
  size_t nrow, ncol;
  Matrix(size_t row, size_t col) {
    this->mat = nullptr;
    init(row, col);
  }
  Matrix() : Matrix(0, 0) {}
  // Matrix(size_t row, size_t col, std::vector<double> const & vec);
  Matrix(const Matrix &m);
  ~Matrix();
  
  double& operator() (size_t i, size_t j) {
    return this->mat[i * (this->ncol) + j];
  }

  double operator() (size_t i, size_t j) const {
    return this->mat[i * (this->ncol) + j];
  }
  
  bool operator== (const Matrix& B) const;
  std::string to_string() const;
  friend Matrix multiply_mkl(const Matrix& A, const Matrix&B);
 private:
  double *mat;
  void init(size_t nrow, size_t ncol);
};

Matrix multiply_naive(const Matrix& A, const Matrix& B);
Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_width);
Matrix multiply_mkl(const Matrix& A, const Matrix& B);

#endif
