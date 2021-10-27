#include "matrix_mul.hpp"

#include <stdexcept>

#include "mkl.h"

const std::string DIM_MISMATCH_ERR_MSG =
    "a's number of columns must equal b's number of rows";

Matrix multiply_naive(const Matrix &a, const Matrix &b) {
  if (a.ncol() != b.nrow())
    throw std::domain_error(DIM_MISMATCH_ERR_MSG);

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

Matrix multiply_tile(const Matrix &a, const Matrix &b, size_t) {
  // TODO: Implement.
  return multiply_naive(a, b);
}

Matrix multiply_mkl(const Matrix &a, const Matrix &b) {
  if (a.ncol() != b.nrow())
    throw std::domain_error(DIM_MISMATCH_ERR_MSG);

  Matrix c(a.nrow(), b.ncol());

  cblas_dgemm(/*Layout=*/CblasRowMajor, /*transa=*/CblasNoTrans,
              /*transb=*/CblasNoTrans, /*m=*/a.nrow(), /*n=*/b.ncol(),
              /*k=*/a.ncol(), /*alpha=*/1, /*a=*/a.data(), /*lda=*/a.ncol(),
              /*b=*/b.data(), /*ldb=*/b.ncol(), /*beta=*/0, /*c=*/c.data(),
              /*ldc=*/c.ncol());

  return c;
}
