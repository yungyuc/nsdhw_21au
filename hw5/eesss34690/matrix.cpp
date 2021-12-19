#include <cstddef>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <algorithm>
#include "mkl.h"
#include "matrix.hpp"
namespace py = pybind11;
using namespace std;


Matrix::Matrix(const vector<vector<double>> &m) {
  m_nrow = m.size();
  m_ncol = m[0].size();
  for (auto &i: m) {
    m_buffer.insert(end(m_buffer), begin(i), end(i));
  }
}

bool operator==(const Matrix &mat1, const Matrix &mat2) {
  if (mat1.m_nrow != mat2.m_nrow || mat1.m_ncol != mat2.m_ncol) {
    return false;
  }
  for (size_t i = 0; i < mat1.m_nrow; ++i) {
    for (size_t j = 0; j < mat2.m_ncol; ++j) {
      if (mat1(i, j) != mat2(i, j)) {
        return false;
      }
    }
  }
  return true;
}

Matrix multiply_naive(const Matrix &m1, const Matrix &m2) {
  if (m1.ncol() != m2.nrow()) {
    throw std::out_of_range(
        "the number of first matrix column "
        "differs from that of second matrix row");
  }

  Matrix m3(m1.nrow(), m2.ncol());
  for (size_t i = 0; i < m1.nrow(); ++i) {
    for (size_t j = 0; j < m2.ncol(); ++j) {
      double v = 0.0;
      for (size_t k = 0; k < m1.ncol(); ++k) {
        v += m1(i, k) * m2(k, j);
      }
      m3(i, j) = v;
    }
  }
  return m3;
}

Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t tsize) {
  if (m1.ncol() != m2.nrow()) {
    throw out_of_range(
        "the number of first matrix column "
        "differs from that of second matrix row");
  }

  size_t bound_i, bound_j, bound_k;
  Matrix m3(m1.nrow(), m2.ncol());
  for (size_t i = 0; i < m1.nrow(); i += tsize)
  {
    for (size_t j = 0; j < m2.ncol(); j += tsize) 
    {
      bound_i = min(m1.nrow(), i + tsize);
      bound_j = min(m2.ncol(), j + tsize);
      for (size_t k = 0; k < m1.ncol(); k += tsize)
      {
        bound_k = min(m1.ncol(), k + tsize);
        for (size_t bk = k; bk < bound_k; ++bk)
	{
          for (size_t bi = i; bi < bound_i; ++bi)
	  {
            for (size_t bj = j; bj < bound_j; ++bj)
	    {
              m3(bi, bj) += m1(bi, bk) * m2(bk, bj);
            }
          }
        }
      }
    }
  }
  return m3;
}

Matrix multiply_mkl(const Matrix &m1, const Matrix &m2) {
  if (m1.ncol() != m2.nrow()) {
    throw out_of_range(
        "the number of first matrix column "
        "differs from that of second matrix row");
  }

  Matrix m3(m1.nrow(), m2.ncol());
  cblas_dgemm(
	CblasRowMajor,
       	CblasNoTrans,
       	CblasNoTrans,
       	m1.nrow(), 
	m2.ncol(),
        m1.ncol(), 
	1.0, 
	m1.buffer(), 
	m1.ncol(), 
	m2.buffer(), 
	m2.ncol(),
        0.0, 
	m3.buffer(), 
	m3.ncol()
  );
  return m3;
}
