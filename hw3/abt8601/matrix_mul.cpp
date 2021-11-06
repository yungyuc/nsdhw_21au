#include "matrix_mul.hpp"

#include <stdexcept>

#include "mkl.h"

const std::string DIM_MISMATCH_ERR_MSG =
    "a's number of columns must equal b's number of rows";

static void check_mul_arg(const Matrix &a, const Matrix &b) {
  if (a.ncol() != b.nrow())
    throw std::domain_error(DIM_MISMATCH_ERR_MSG);
}

Matrix multiply_naive(const Matrix &a, const Matrix &b) {
  check_mul_arg(a, b);

  Matrix c(a.nrow(), b.ncol());

  for (size_t i = 0; i < a.nrow(); i++) {
    for (size_t j = 0; j < b.ncol(); j++) {
      double cij = 0;
      for (size_t k = 0; k < a.ncol(); k++)
        cij += a[std::make_pair(i, k)] * b[std::make_pair(k, j)];
      c[std::make_pair(i, j)] = cij;
    }
  }

  return c;
}

static void multiply_single_tile(const Matrix &a, const Matrix &b,
                                 const size_t il, const size_t iu,
                                 const size_t jl, const size_t ju,
                                 const size_t kl, const size_t ku, Matrix &c) {
  for (size_t i = il; i < iu; i++) {
    for (size_t j = jl; j < ju; j++) {
      double cij = c[std::make_pair(i, j)];
      for (size_t k = kl; k < ku; k++)
        cij += a[std::make_pair(i, k)] * b[std::make_pair(k, j)];
      c[std::make_pair(i, j)] = cij;
    }
  }
}

Matrix multiply_tile(const Matrix &a, const Matrix &b, const size_t tile_size) {
  check_mul_arg(a, b);

  if (tile_size == 0)
    throw std::logic_error("tile_size must be positive");

  Matrix c = Matrix::zeros(a.nrow(), b.ncol());

  for (size_t i = 0; i < a.nrow(); i += tile_size) {
    for (size_t j = 0; j < b.ncol(); j += tile_size) {
      for (size_t k = 0; k < a.ncol(); k += tile_size) {
        multiply_single_tile(a, b, i, std::min(i + tile_size, a.nrow()), j,
                             std::min(j + tile_size, b.ncol()), k,
                             std::min(k + tile_size, a.ncol()), c);
      }
    }
  }

  return c;
}

Matrix multiply_mkl(const Matrix &a, const Matrix &b) {
  check_mul_arg(a, b);

  if (a.ncol() == 0)
    return Matrix::zeros(a.nrow(), b.ncol());
  if (b.ncol() == 0)
    return Matrix(a.nrow(), 0);

  Matrix c(a.nrow(), b.ncol());

  cblas_dgemm(/*Layout=*/CblasRowMajor, /*transa=*/CblasNoTrans,
              /*transb=*/CblasNoTrans, /*m=*/a.nrow(), /*n=*/b.ncol(),
              /*k=*/a.ncol(), /*alpha=*/1, /*a=*/a.data(), /*lda=*/a.ncol(),
              /*b=*/b.data(), /*ldb=*/b.ncol(), /*beta=*/0, /*c=*/c.data(),
              /*ldc=*/c.ncol());

  return c;
}
