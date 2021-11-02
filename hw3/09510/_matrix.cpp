#include "_matrix.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace std;

Matrix multiply_naive(Matrix const & a, Matrix const & b)
{

    Matrix ans(a.nrow(), b.ncol());


    for (size_t i = 0; i < a.nrow(); ++i) {
        for (size_t j = 0; j < b.ncol(); ++j) {
            double v = 0;
            for (size_t k = 0; k < b.nrow(); ++k) {
                v += a(i, k) * b(k, j);
            }
            ans(i, j) = v;
        }

    }

    return ans;

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

Matrix multiply_mkl(Matrix const & a, Matrix const & b)
{

    Matrix ans = Matrix(a.nrow(), b.ncol());

    cblas_dgemm(
         CblasRowMajor
        ,CblasNoTrans
        ,CblasNoTrans
        ,a.nrow()
        ,b.ncol()
        ,a.ncol()
        ,1.0
        ,a.get_matrix_buffer()
        ,a.ncol()
        ,b.get_matrix_buffer()
        ,b.ncol()
        ,0.0
        ,ans.get_matrix_buffer()
        ,ans.ncol()
    );

    return ans;
} 


PYBIND11_MODULE(_matrix, m) {
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);

  py::class_<Matrix>(m, "Matrix")
    .def(py::init<size_t, size_t>())
    //.def(py::init<const Matrix &matrix> &>())
    .def_property_readonly("nrow", &Matrix::nrow)
    .def_property_readonly("ncol", &Matrix::ncol)

    .def("__eq__", [](const Matrix &self, const Matrix &other) { return self == other; })
    .def("assign", &Matrix::operator=)
    .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> idx, double val) { return self(idx.first, idx.second) = val; })
    .def("__getitem__", [](const Matrix &self, std::pair<size_t, size_t> idx) { return self(idx.first, idx.second); });
}