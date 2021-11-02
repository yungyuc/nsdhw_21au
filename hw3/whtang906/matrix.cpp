
#include <algorithm>
#include <cstddef>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <mkl.h>

using namespace std;

class Matrix
{
public:
  Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0.0)
  {
  }

  Matrix(const vector<vector<double>> &target)
  {
    m_nrow = target.size();
    m_ncol = target[0].size();
    for (auto &i : target)
    {
      m_buffer.insert(end(m_buffer), begin(i), end(i));
    }
  }
  // No bound check.
  double operator()(size_t row, size_t col) const
  {
    return m_buffer[row * m_ncol + col];
  }
  double &operator()(size_t row, size_t col)
  {
    return m_buffer[row * m_ncol + col];
  }

  bool operator==(const Matrix &other) const
  {
    for (size_t i = 0; i < m_nrow; ++i)
    {
      for (size_t j = 0; j < m_ncol; ++j)
      {
        if (m_buffer[i * m_ncol, j] != other(i, j))
        {
          return false;
        }
      }
    }
    return true;
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

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
  Matrix ret(mat1.nrow(), mat2.ncol());

  for (size_t i = 0; i < mat1.nrow(); ++i)
  {
    for (size_t k = 0; k < mat2.ncol(); ++k)
    {
      double v = 0;
      for (size_t j = 0; j < mat1.ncol(); ++j)
      {
        v += mat1(i, j) * mat2(j, k);
      }
      ret(i, k) = v;
    }
  }
  return ret;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tsize)
{
  Matrix ret(mat1.nrow(), mat2.ncol());

  for (size_t i = 0; i < mat1.nrow(); i += tsize)
  {
    for (size_t k = 0; k < mat2.ncol(); k += tsize)
    {
      for (size_t j = 0; j < mat1.ncol(); j += tsize)
      {
        for (size_t ti = 0; ti < min(mat1.nrow(), i + tsize); ++ti)
        {
          for (size_t tk = 0; tk < min(mat2.ncol(), k + tsize); ++tk)
          {
            double v = 0;
            for (size_t tj = 0; tj < min(mat1.ncol(), j + tsize); ++tj)
            {
              v += mat1(ti, tj) * mat2(tj, tk);
            }
            ret(ti, tk) = v;
          }
        }
      }
    }
  }

  return ret;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
  Matrix ret(mat1.nrow(), mat2.ncol());

  cblas_dgemm(
      CblasRowMajor /* const CBLAS_LAYOUT Layout */
      ,
      CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      ,
      CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      ,
      mat1.nrow() /* const MKL_INT m */
      ,
      mat2.ncol() /* const MKL_INT n */
      ,
      mat1.ncol() /* const MKL_INT k */
      ,
      1.0 /* const double alpha */
      ,
      mat1.buffer() /* const double *a */
      ,
      mat1.ncol() /* const MKL_INT lda */
      ,
      mat2.buffer() /* const double *b */
      ,
      mat2.ncol() /* const MKL_INT ldb */
      ,
      0.0 /* const double beta */
      ,
      ret.buffer() /* double * c */
      ,
      ret.ncol() /* const MKL_INT ldc */
  );

  return ret;
}

PYBIND11_MODULE(_matrix, m)
{
  m.doc() = "3 types of matrix-matrix multiplication";
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);

  pybind11::class_<Matrix>(m, "Matrix")
      .def(pybind11::init<size_t, size_t>())
      .def(pybind11::init<const vector<vector<double>> &>())
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)
      .def("__eq__", [](const Matrix &mat, const Matrix &other)
           { return mat == other; })
      .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> idx, double val)
           { return mat(idx.first, idx.second) = val; })
      .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> idx)
           { return mat(idx.first, idx.second); });
}