#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <mkl.h>
#include "CustomAllocator.h"

static CustomAllocator<double> custom_allocator;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(custom_allocator)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix & operator=(Matrix const & other)
    {
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        return *this;
    }

    Matrix(Matrix && other)
      : m_buffer(std::move(other.m_buffer))
    {
        m_buffer.clear();
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        m_buffer.clear();
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    bool operator==(Matrix const &other) const
    {
        if ((m_nrow != other.nrow()) || (m_ncol != other.ncol()))
            return false;

        for (size_t i = 0; i < m_nrow; i++)
            for (size_t j = 0; j < m_ncol; j++)
                if (m_buffer[index(i, j)] != other(i, j))
                    return false;

        return true;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }


    double get_data(std::pair<size_t, size_t> coord)
    {
        return m_buffer[index(coord.first, coord.second)];
    }
    void set_data(std::pair<size_t, size_t> coord, const double val)
    {
        m_buffer[index(coord.first, coord.second)] = val;
    }


    double buffer(size_t i) const { return m_buffer[i]; }
    double &buffer(size_t i)  { return m_buffer[i]; }

    double* buffer_data() { return m_buffer.data(); }
    const double* buffer_data() const { return m_buffer.data(); }

private:

    size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        m_buffer.resize(nrow * ncol, 0.0);

        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, CustomAllocator<double>> m_buffer;

};

/*
 * Naive matrix matrix multiplication.
 */
Matrix operator*(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;
}

std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        ostr << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << " " << std::setw(2) << mat(i, j);
        }
    }

    return ostr;
}


Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
  if (mat1.ncol() != mat2.nrow())
  {
    throw std::out_of_range(
        "the matrices are not multipliable");
  }
  Matrix ret(mat1.nrow(), mat2.ncol());

  for (size_t i = 0; i < mat1.nrow(); ++i)
  {
    for (size_t j = 0; j < mat2.ncol(); ++j)
    {
      double v = 0;
      for (size_t k = 0; k < mat1.ncol(); ++k)
      {
        v += mat1(i, k) * mat2(k, j);
      }
      ret(i, j) = v;
    }
  }
  return ret;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tsize)
{
  if (mat1.ncol() != mat2.nrow())
  {
    throw std::out_of_range(
        "the matrices are not multipliable");
  }
  Matrix ret(mat1.nrow(), mat2.ncol());

  for (size_t i = 0; i < mat1.nrow(); i += tsize)
  {
    for (size_t j = 0; j < mat2.ncol(); j += tsize)
    {
      for (size_t k = 0; k < mat1.ncol(); k += tsize)
      {
        for (size_t tk = k; tk < std::min(mat1.nrow(), k + tsize); ++tk)
        {
          for (size_t ti = i; ti < std::min(mat2.ncol(), i + tsize); ++ti)
          {
            for (size_t tj = j; tj < std::min(mat1.ncol(), j + tsize); ++tj)
            {
              // ret(ti, tj) += mat1(ti, tk) * mat2(tk, tj);
              ret.buffer(tk * mat1.ncol() + ti) += mat1.buffer(tk * mat1.ncol() + tj) * mat2.buffer(tj * mat1.ncol() + ti);
            }
          }
        }
      }
    }
  }

  return ret;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
  if (mat1.ncol() != mat2.nrow())
  {
    throw std::out_of_range(
        "the matrices are not multipliable");
  }
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
      mat1.buffer_data() /* const double *a */
      ,
      mat1.ncol() /* const MKL_INT lda */
      ,
      mat2.buffer_data() /* const double *b */
      ,
      mat2.ncol() /* const MKL_INT ldb */
      ,
      0.0 /* const double beta */
      ,
      ret.buffer_data() /* double * c */
      ,
      ret.ncol() /* const MKL_INT ldc */
  );

  return ret;
}

std::size_t bytes() { return custom_allocator.bytes(); }
std::size_t allocated() { return custom_allocator.allocated(); }
std::size_t deallocated() { return custom_allocator.deallocated(); }

PYBIND11_MODULE(_matrix, m)
{
  m.doc() = "3 types of matrix-matrix multiplication";
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);
  m.def("bytes", &bytes);
  m.def("allocated", &allocated);
  m.def("deallocated", &deallocated);

  pybind11::class_<Matrix>(m, "Matrix")
      .def(pybind11::init<size_t, size_t>())
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)
      .def("__setitem__", &Matrix::set_data)
      .def("__getitem__", &Matrix::get_data)
      .def(pybind11::self == pybind11::self);
}