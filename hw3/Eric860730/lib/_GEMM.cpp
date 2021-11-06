#include <iostream>
#include <stdexcept>

#include <mkl.h>

#include "../include/_GEMM.hpp"

void validate_multiplication(Matrix const &mat1, Matrix const &mat2) {
  if (mat1.ncol() != mat2.nrow()) {
    throw std::out_of_range(
        "the number of first matrix column "
        "differs from that of second matrix row");
  }
}

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2) {
  validate_multiplication(mat1, mat2);
  Matrix m_result(mat1.nrow(), mat2.ncol());

  // row-major
  for (size_t i = 0; i < mat1.nrow(); ++i) {
    for (size_t j = 0; j < mat2.ncol(); ++j) {
      double v = 0;
      for (size_t k = 0; k < mat2.nrow(); ++k) {
        v += mat1(i, k) * mat2(k, j);
      }
      m_result(i, j) = v;
      //printf("m_result(%d,%d) = {%f}", i, j, m_result(i,j));
    }
  }

  return m_result;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tile_size) {
  validate_multiplication(mat1, mat2);
  const size_t m1_row = mat1.nrow();
  const size_t m2_col = mat2.ncol();
  const size_t m2_row = mat2.nrow();

  Matrix m_result(m1_row, m2_col);
  for (size_t i = 0; i < m1_row; ++i) {
    for (size_t j = 0; j < m2_col; ++j) {
      m_result(i, j) = 0;
    }
  }

  for (size_t i = 0; i < m1_row; i+=tile_size) {
    for (size_t j = 0; j < m2_col; j+=tile_size) {
      for (size_t k = 0; k < m2_row; k+=tile_size) {
        // tile(i, j) = tile(j, k) * tile(k, i)
        // calculate tile(i, j) with inner loop.
        for (size_t ti = i; ti < std::min(i + tile_size, m1_row); ++ti) {
          for (size_t tj = j; tj < std::min(j + tile_size, m2_col); ++tj) {
            double val = 0;
            for (size_t tk = k; tk < std::min(k + tile_size, m2_row); ++tk) {
              val += mat1(ti, tk) * mat2(tk, tj);
            }
            m_result(ti, tj) += val;
          }
        }
      }
    }
  }

  return m_result;

}

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
  validate_multiplication(mat1, mat2);

  mkl_set_num_threads(1);

  Matrix ret(mat1.nrow(), mat2.ncol());

  cblas_dgemm(
      CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , mat1.nrow() /* const MKL_INT m */
      , mat2.ncol() /* const MKL_INT n */
      , mat1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , mat1.raw_data() /* const double *a */
      , mat1.ncol() /* const MKL_INT lda */
      , mat2.raw_data() /* const double *b */
      , mat2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.raw_data() /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
      );

  std::cout << ret(0,0) << std::endl;
  return ret;
}
