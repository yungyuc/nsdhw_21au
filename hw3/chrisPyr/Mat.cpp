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
  Matrix res(A_mat.row(), B_mat.col());
  for (size_t i = 0; i < A_mat.row(); ++i) {
    for (size_t j = 0; j < B_mat.col(); ++j) {
      double result = 0;
      for (size_t k = 0; k < A_mat.col(); ++k) {
        result += A_mat.getdata(i, k) * B_mat.getdata(k, j);
      }
      res.getpos(i, j) = result;
    }
  }
  return res;
}

Matrix multiply_tile(Matrix &A_mat, Matrix &B_mat){
   size_t row1 = A_mat.row();
   size_t col = B_mat.col();
   size_t row2 = A_mat.col();
  Matrix res(A_mat.row(), B_mat.col());
size_t incr = 64;
    for (size_t i = 0; i < row1; i += incr) {
         for (size_t j = 0; j < col; j += incr) {
             res.getpos(i,j) = 0.0;
             for (size_t k = 0; k < row2; k += incr) {
                 for (size_t x = i; x < std::min( i + incr, row1 ); x++) {
                     for (size_t y = j; y < std::min( j + incr, col ); y++) {
                         for (size_t z = k; z < std::min( k + incr, row2 ); z++) {

                             res.getpos(x,y) +=  A_mat.getdata(x,z) * B_mat.getdata(z,y);

                         }
                     } 
                 }
             }
         }
     }
return res;
}

Matrix multiply_mkl(Matrix &A_mat, Matrix &B_mat){
//    mkl_set_num_threads(1);

    Matrix ret(A_mat.row(), B_mat.col());


    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , A_mat.row() /* const MKL_INT m */
      , B_mat.col() /* const MKL_INT n */
      , A_mat.col() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , A_mat.data /* const double *a */
      , A_mat.col() /* const MKL_INT lda */
      , B_mat.data /* const double *b */
      , B_mat.col() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.data /* double * c */
      , ret.col() /* const MKL_INT ldc */
    );

    return ret;
}
