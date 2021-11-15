#include <mkl.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

namespace py = pybind11;

class ByteCounter {
  // TODO
};

template <class T> struct CustomAllocator {
  // TODO
};

struct Matrix {
  size_t nrow = 0;
  size_t ncol = 0;
  std::vector<double, CustomAllocator<double>> buffer;
  Matrix(size_t nrow, size_t ncol) : nrow(nrow), ncol(ncol) {
    reset_buffer(nrow, ncol);
  }
  Matrix &operator=(std::vector<double> const &vec) {
    if (size() != vec.size()) {
      throw std::out_of_range("number of elements mismatch");
    }
    size_t k = 0;
    for (size_t i = 0; i < nrow; ++i) {
      for (size_t j = 0; j < ncol; ++j) {
        (*this)(i, j) = vec[k];
        ++k;
      }
    }
    return *this;
  }
  Matrix &operator=(Matrix const &other) {
    if (this == &other) {
      return *this;
    }
    if (nrow != other.nrow || ncol != other.ncol) {
      reset_buffer(other.nrow, other.ncol);
    }
    for (size_t i = 0; i < nrow; ++i) {
      for (size_t j = 0; j < ncol; ++j) {
        (*this)(i, j) = other(i, j);
      }
    }
    return *this;
  }
  ~Matrix() { reset_buffer(0, 0); }
  size_t index(size_t row, size_t col) const { return row * ncol + col; }
  double operator()(size_t row, size_t col) const {
    return buffer[index(row, col)];
  }
  double &operator()(size_t row, size_t col) { return buffer[index(row, col)]; }
  bool operator==(const Matrix &m) const {
    if (nrow != m.nrow || ncol != m.ncol) {
      return false;
    }
    for (int i = 0; i < nrow; ++i) {
      for (int j = 0; j < ncol; ++j) {
        if ((*this)(i, j) - m(i, j) < -1e-4 or (*this)(i, j) - m(i, j) > 1e-4) {
          return false;
        }
      }
    }
    return true;
  }
  double operator[](size_t idx) const { return buffer[idx]; }
  double &operator[](size_t idx) { return buffer[idx]; }
  size_t size() const { return nrow * ncol; }
  void reset_buffer(size_t nrow, size_t ncol) {
    const size_t nelement = nrow * ncol;
    buffer.resize(nelement, 0);
    nrow = nrow;
    ncol = ncol;
  }
};

size_t bytes() {
  // TODO
}

size_t allocated() {
  // TODO
}

size_t deallocated() {
  // TODO
}

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "matrix memory management";
  m.def("bytes", &bytes);
  m.def("allocated", &allocated);
  m.def("deallocated", &deallocated);
  py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def_readwrite("nrow", &Matrix::nrow)
      .def_readwrite("ncol", &Matrix::ncol)
      .def(
          "__getite_",
          [](Matrix &mat, std::pair<size_t, size_t> p) {
            return mat(p.first, p.second);
          },
          py::is_operator())
      .def(
          "__setite_",
          [](Matrix &mat, std::pair<size_t, size_t> p, double val) {
            mat(p.first, p.second) = val;
          },
          py::is_operator())
      .def("__eq__", &Matrix::operator==)
      .def(py::self == py::self);
}