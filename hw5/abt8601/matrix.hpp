#pragma once
#ifndef NSDHW_21AU_HW5_MATRIX_HPP
#define NSDHW_21AU_HW5_MATRIX_HPP

#include <algorithm>
#include <cstddef>
#include <sstream>
#include <string>
#include <utility>

class Matrix {
public:
  Matrix() = delete;

  Matrix(const Matrix &other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
    if (other.m_elems) {
      const std::size_t nelems = m_nrow * m_ncol;
      m_elems = new double[nelems];
      std::copy_n(other.m_elems, nelems, m_elems);
    } else {
      m_elems = nullptr;
    }
  }

  Matrix(Matrix &&other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_elems(other.m_elems) {
    other.m_nrow = 0;
    other.m_ncol = 0;
    other.m_elems = nullptr;
  }

  Matrix &operator=(const Matrix &other) {
    if (this == &other)
      return *this;

    m_nrow = other.m_nrow;
    m_ncol = other.m_ncol;

    if (m_elems)
      delete[] m_elems;

    if (other.m_elems) {
      const std::size_t nelems = m_nrow * m_ncol;
      m_elems = new double[nelems];
      std::copy_n(other.m_elems, nelems, m_elems);
    } else {
      m_elems = nullptr;
    }

    return *this;
  }

  Matrix &operator=(Matrix &&other) {
    if (this == &other)
      return *this;

    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_elems, other.m_elems);

    return *this;
  }

  virtual ~Matrix() {
    if (m_elems)
      delete[] m_elems;
  }

  Matrix(const std::size_t nrow, const std::size_t ncol)
      : m_nrow(nrow), m_ncol(ncol) {
    const std::size_t nelems = m_nrow * m_ncol;
    if (nelems > 0) {
      m_elems = new double[nelems];
    } else {
      m_elems = nullptr;
    }
  }

  static Matrix zeros(const std::size_t nrow, const std::size_t ncol) {
    const std::size_t nelems = nrow * ncol;
    double *elems = nullptr;
    if (nelems > 0)
      elems = new double[nelems]();

    return Matrix(nrow, ncol, elems);
  }

  std::size_t nrow() const noexcept { return m_nrow; }
  std::size_t ncol() const noexcept { return m_ncol; }
  const double *data() const noexcept { return m_elems; }
  double *data() noexcept { return m_elems; }

  double
  operator[](const std::pair<std::size_t, std::size_t> ix) const noexcept {
    const std::size_t i = ix.first, j = ix.second;
    return m_elems[i * m_ncol + j];
  }

  double &operator[](const std::pair<std::size_t, std::size_t> ix) noexcept {
    const std::size_t i = ix.first, j = ix.second;
    return m_elems[i * m_ncol + j];
  }

  double operator()(const std::size_t i, const std::size_t j) const {
    check_ix(i, j);
    return m_elems[i * m_ncol + j];
  }

  double &operator()(const std::size_t i, const std::size_t j) {
    check_ix(i, j);
    return m_elems[i * m_ncol + j];
  }

  bool operator==(const Matrix &other) const noexcept {
    if (m_nrow == other.m_nrow && m_ncol == other.m_ncol) {
      const std::size_t nelems = m_nrow * m_ncol;
      return std::equal(m_elems, m_elems + nelems, other.m_elems);
    } else {
      return false;
    }
  }

protected:
  Matrix(const std::size_t nrow, const std::size_t ncol,
         double *const elems) noexcept
      : m_nrow(nrow), m_ncol(ncol), m_elems(elems) {}

  void check_ix(const std::size_t i, const std::size_t j) const {
    if (i >= m_nrow || j >= m_ncol)
      throw std::out_of_range("matrix index out of range");
  }

  std::size_t m_nrow, m_ncol;
  double *m_elems;
};

Matrix multiply_naive(const Matrix &a, const Matrix &b);
Matrix multiply_tile(const Matrix &a, const Matrix &b, std::size_t tile_size);
Matrix multiply_mkl(const Matrix &a, const Matrix &b);

#endif
