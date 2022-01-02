#ifndef NSDHW_21AU_HW6_MATRIX_MUL_HPP
#define NSDHW_21AU_HW6_MATRIX_MUL_HPP

#include "matrix.hpp"

Matrix multiply_naive(const Matrix &a, const Matrix &b);
Matrix multiply_tile(const Matrix &a, const Matrix &b, size_t tile_size);
Matrix multiply_mkl(const Matrix &a, const Matrix &b);

#endif
