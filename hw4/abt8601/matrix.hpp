#pragma once
#ifndef NSDHW_21AU_HW4_MATRIX_HPP
#define NSDHW_21AU_HW4_MATRIX_HPP

#include <cstddef>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "allocator.hpp"

class Matrix {
public:
  Matrix() : m_nrow(0), m_ncol(0) {}

  Matrix(const Matrix &other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_elems(other.m_elems) {}

  Matrix(Matrix &&other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol),
        m_elems(std::move(other.m_elems)) {}

  Matrix &operator=(const Matrix &other) {
    if (this == &other)
      return *this;

    m_nrow = other.m_nrow;
    m_ncol = other.m_ncol;
    m_elems = other.m_elems;

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

  virtual ~Matrix() = default;

  Matrix(const size_t nrow, const size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_elems(nrow * ncol) {}

  static Matrix zeros(const size_t nrow, const size_t ncol) {
    return Matrix(
        nrow, ncol,
        std::vector<double, ByteCountAllocator<double>>(nrow * ncol, 0));
  }

  size_t nrow() const noexcept { return m_nrow; }
  size_t ncol() const noexcept { return m_ncol; }
  const double *data() const noexcept { return m_elems.data(); }
  double *data() noexcept { return m_elems.data(); }

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
  Matrix(const size_t nrow, const size_t ncol,
         std::vector<double, ByteCountAllocator<double>> &&elems) noexcept
      : m_nrow(nrow), m_ncol(ncol), m_elems(std::move(elems)) {}

  void check_ix(const std::pair<size_t, size_t> ix) const {
    const size_t i = ix.first, j = ix.second;
    if (i >= m_nrow || j >= m_ncol)
      throw std::out_of_range("matrix index out of range");
  }

  size_t m_nrow, m_ncol;
  std::vector<double, ByteCountAllocator<double>> m_elems;
};

#endif
