#pragma once
#ifndef NSDHW_21AU_HW3_MATRIX_HPP
#define NSDHW_21AU_HW3_MATRIX_HPP

#include <cstddef>
#include <string>
#include <utility>

class Matrix {
public:
  Matrix();
  Matrix(const Matrix &);
  Matrix(Matrix &&);
  Matrix &operator=(const Matrix &);
  Matrix &operator=(Matrix &&);

  virtual ~Matrix();

  Matrix(size_t nrow, size_t ncol);

  static Matrix zeros(size_t nrow, size_t ncol);

  size_t nrow() const noexcept;
  size_t ncol() const noexcept;
  const double *data() const noexcept;
  double *data() noexcept;

  double operator[](std::pair<size_t, size_t>) const noexcept;
  double &operator[](std::pair<size_t, size_t>) noexcept;

  double get_elem(std::pair<size_t, size_t>) const;
  void set_elem(std::pair<size_t, size_t>, double);

  bool operator==(const Matrix &) const noexcept;
  bool operator!=(const Matrix &) const noexcept;

  std::string repr() const;

protected:
  Matrix(size_t nrow, size_t ncol, double *elems) noexcept;

  void check_ix(std::pair<size_t, size_t>) const;

  size_t m_nrow, m_ncol;
  double *m_elems;
};

#endif
