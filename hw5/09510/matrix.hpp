#include <algorithm>
#include "mkl.h"
#include <cstddef>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace pybind = pybind11;
using namespace std;



class Matrix {
  public:
    Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0.0) {}
    Matrix(const vector<vector<double>> &m)
    {
      m_nrow = m.size();
      m_ncol = m[0].size();
      for (auto &i: m) {
        m_buffer.insert(end(m_buffer), begin(i), end(i));
      }
    }

    bool operator==(Matrix const &other)const
    {
      if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
      {
        return false;
      }
      for (size_t i = 0; i < m_nrow; ++i)
      {
        for (size_t j = 0; j < m_ncol; ++j)
        {
          if ((*this)(i, j) != other(i, j)) {return false;}
        }
      }
      return true;
    }


    double operator()(size_t row, size_t col) const
    {
      return m_buffer[row * m_ncol + col];
    }
    double &operator()(size_t row, size_t col)
    {
      return m_buffer[row * m_ncol + col];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double *buffer() { return m_buffer.data(); }
    const double *buffer() const { return m_buffer.data(); }

  private:
    size_t m_nrow;
    size_t m_ncol;
    vector<double> m_buffer;
};




Matrix multiply_naive(const Matrix &a, const Matrix &b) {
  if(a.ncol()!=b.nrow()){throw pybind::value_error("these two matric can't multiply.");}

  Matrix ans(a.nrow(), b.ncol());
  for (size_t i = 0; i < a.nrow(); ++i) 
  {
    for (size_t j = 0; j < b.ncol(); ++j) 
    {

      double v = 0.0;
      for (size_t k = 0; k < a.ncol(); ++k) 
      {
        v += a(i, k) * b(k, j);
      }
      ans(i, j) = v;

    }
  }
  return ans;
}



Matrix multiply_tile(const Matrix &a, const Matrix &b, size_t tsize) {
  
  
  if(a.ncol()!=b.nrow()){throw pybind::value_error("these two matric can't multiply.");}
  Matrix ans(a.nrow(), b.ncol());
  for (size_t i = 0; i < a.nrow(); i += tsize)
  {
    for (size_t j = 0; j < b.ncol(); j += tsize) 
    {
      for (size_t k = 0; k < a.ncol(); k += tsize)
      {
        for (size_t t_k = k; t_k < min(a.ncol(), k + tsize); t_k++)
	      {
          for (size_t t_i = i; t_i < min(a.nrow(), i + tsize); t_i++)
	        {   
            for (size_t t_j = j; t_j < min(a.ncol(), j + tsize); t_j++)
	          {
              ans(t_i, t_j) += a(t_i, t_k) * b(t_k, t_j);
            }
          }
        }
      }
    }
  }
  return ans;
}

Matrix multiply_mkl(const Matrix &a, const Matrix &b) {
  
  if(a.ncol()!=b.nrow()){throw pybind::value_error("these two matric can't multiply.");}
  Matrix ans(a.nrow(), b.ncol());
  cblas_dgemm(
	  CblasRowMajor,
    CblasNoTrans,
    CblasNoTrans,
    a.nrow(), 
    b.ncol(),
    a.ncol(), 
    1.0, 
    a.buffer(), 
    a.ncol(), 
    b.buffer(), 
    b.ncol(),
    0.0, 
    ans.buffer(), 
    ans.ncol()
  );
  return ans;
}



