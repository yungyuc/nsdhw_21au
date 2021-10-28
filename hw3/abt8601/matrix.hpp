#pragma once
#ifndef NSDHW_21AU_HW3_MATRIX_HPP
#define NSDHW_21AU_HW3_MATRIX_HPP

#include <algorithm>
#include <cstddef>
#include <sstream>
#include <string>
#include <utility>

class Matrix {
public:
  Matrix() : m_nrow(0), m_ncol(0), m_elems(nullptr) {}

  Matrix(const Matrix &other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
    if (other.m_elems) {
      const size_t nelems = m_nrow * m_ncol;
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
      const size_t nelems = m_nrow * m_ncol;
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

  Matrix(const size_t nrow, const size_t ncol) : m_nrow(nrow), m_ncol(ncol) {
    const size_t nelems = m_nrow * m_ncol;
    if (nelems > 0) {
      m_elems = new double[nelems];
    } else {
      m_elems = nullptr;
    }
  }

  static Matrix zeros(const size_t nrow, const size_t ncol) {
    const size_t nelems = nrow * ncol;
    double *elems = nullptr;
    if (nelems > 0)
      elems = new double[nelems]();

    return Matrix(nrow, ncol, elems);
  }

  size_t nrow() const noexcept { return m_nrow; }
  size_t ncol() const noexcept { return m_ncol; }
  const double *data() const noexcept { return m_elems; }
  double *data() noexcept { return m_elems; }

  double operator[](const std::pair<size_t, size_t> ix) const noexcept {
    const size_t i = ix.first, j = ix.second;
    return m_elems[i * m_ncol + j];
  }

  double &operator[](const std::pair<size_t, size_t> ix) noexcept {
    const size_t i = ix.first, j = ix.second;
    return m_elems[i * m_ncol + j];
  }

  double get_elem(const std::pair<size_t, size_t> ix) const {
    check_ix(ix);
    return (*this)[ix];
  }

  void set_elem(const std::pair<size_t, size_t> ix, const double v) {
    check_ix(ix);
    (*this)[ix] = v;
  }

  bool operator==(const Matrix &other) const noexcept {
    if (m_nrow == other.m_nrow && m_ncol == other.m_ncol) {
      for (size_t i = 0; i < m_nrow; i++) {
        for (size_t j = 0; j < m_ncol; j++) {
          const std::pair<size_t, size_t> ix = std::make_pair(i, j);
          if ((*this)[ix] != other[ix])
            return false;
        }
      }
      return true;
    } else {
      return false;
    }
  }

  bool operator!=(const Matrix &other) const noexcept {
    return !(*this == other);
  }

  std::string repr() const {
    std::ostringstream ss;

    for (size_t i = 0; i < m_nrow; i++) {
      ss << (i == 0 ? '[' : ' ');
      ss << '[';

      for (size_t j = 0; j < m_ncol; j++) {
        if (j != 0)
          ss << ", ";
        ss << (*this)[std::make_pair(i, j)];
      }

      ss << ']';
      ss << (i == m_nrow - 1 ? ']' : '\n');
    }

    return ss.str();
  }

protected:
  Matrix(const size_t nrow, const size_t ncol, double *const elems) noexcept
      : m_nrow(nrow), m_ncol(ncol), m_elems(elems) {}

  void check_ix(const std::pair<size_t, size_t> ix) const {
    const size_t i = ix.first, j = ix.second;
    if (i >= m_nrow || j >= m_ncol)
      throw std::out_of_range("matrix index out of range");
  }

  size_t m_nrow, m_ncol;
  double *m_elems;
};

#endif
