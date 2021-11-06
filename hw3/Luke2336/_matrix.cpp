#include <mkl.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

namespace py = pybind11;

struct Matrix {

public:
  Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol) {
    reset_buffer(nrow, ncol);
  }

  Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
      : m_nrow(nrow), m_ncol(ncol) {
    reset_buffer(nrow, ncol);
    (*this) = vec;
  }

  Matrix(std::vector<std::vector<double>> const &vec)
      : m_nrow(vec.size()), m_ncol(vec[0].size()) {
    for (size_t i = 0; i < m_nrow; ++i) {
      for (size_t j = 0; j < m_ncol; ++j) {
        reset_buffer(m_nrow, m_ncol);
        (*this)(i, j) = vec[i][j];
      }
    }
  }

  Matrix &operator=(std::vector<double> const &vec) {
    if (size() != vec.size()) {
      throw std::out_of_range("number of elements mismatch");
    }
    size_t k = 0;
    for (size_t i = 0; i < m_nrow; ++i) {
      for (size_t j = 0; j < m_ncol; ++j) {
        (*this)(i, j) = vec[k];
        ++k;
      }
    }
    return *this;
  }

  Matrix(Matrix const &other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_elapsed(other.m_elapsed),
        m_nflo(other.m_nflo) {
    reset_buffer(other.m_nrow, other.m_ncol);
    for (size_t i = 0; i < m_nrow; ++i) {
      for (size_t j = 0; j < m_ncol; ++j) {
        (*this)(i, j) = other(i, j);
      }
    }
  }

  Matrix &operator=(Matrix const &other) {
    if (this == &other) {
      return *this;
    }
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
      reset_buffer(other.m_nrow, other.m_ncol);
    }
    for (size_t i = 0; i < m_nrow; ++i) {
      for (size_t j = 0; j < m_ncol; ++j) {
        (*this)(i, j) = other(i, j);
      }
    }
    m_elapsed = other.m_elapsed;
    m_nflo = other.m_nflo;
    return *this;
  }

  Matrix(Matrix &&other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_elapsed(other.m_elapsed),
        m_nflo(other.m_nflo) {
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
  }

  Matrix &operator=(Matrix &&other) {
    if (this == &other) {
      return *this;
    }
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
    std::swap(m_elapsed, other.m_elapsed);
    std::swap(m_nflo, other.m_nflo);
    return *this;
  }

  ~Matrix() { reset_buffer(0, 0); }

  double operator()(size_t row, size_t col) const {
    return m_buffer[index(row, col)];
  }
  double &operator()(size_t row, size_t col) {
    return m_buffer[index(row, col)];
  }

  bool operator==(const Matrix &m) const {
    if (m_nrow != m.m_nrow || m_ncol != m.m_ncol) {
      return false;
    }
    for (int i = 0; i < m_nrow; ++i) {
      for (int j = 0; j < m_ncol; ++j) {
        if ((*this)(i, j) != m(i, j)) {
          return false;
        }
      }
    }
    return true;
  }

  double operator[](size_t idx) const { return m_buffer[idx]; }
  double &operator[](size_t idx) { return m_buffer[idx]; }

  size_t nrow() const { return m_nrow; }
  size_t ncol() const { return m_ncol; }

  size_t size() const { return m_nrow * m_ncol; }
  double buffer(size_t i) const { return m_buffer[i]; }
  std::vector<double> buffer_vector() const {
    return std::vector<double>(m_buffer, m_buffer + size());
  }

  double elapsed() const { return m_elapsed; }
  double &elapsed() { return m_elapsed; }

  size_t nflo() const { return m_nflo; }
  size_t &nflo() { return m_nflo; }

  double gflops() const { return m_nflo / m_elapsed / 1.e9; }

  Matrix transpose() const;

public:
  size_t index(size_t row, size_t col) const { return row * m_ncol + col; }

  void reset_buffer(size_t nrow, size_t ncol) {
    if (m_buffer) {
      delete[] m_buffer;
    }
    const size_t nelement = nrow * ncol;
    if (nelement) {
      m_buffer = new double[nelement];
    } else {
      m_buffer = nullptr;
    }
    m_nrow = nrow;
    m_ncol = ncol;
  }

  size_t m_nrow = 0;
  size_t m_ncol = 0;
  double *m_buffer = nullptr;
  double m_elapsed = 0;
  size_t m_nflo = 0; // number of floating-point operations.
};

Matrix multiply_naive(const Matrix &ma, const Matrix &mb) {
  if (ma.m_ncol != mb.m_nrow) {
    throw py::value_error("Matrices cannot be multiplied!");
  }
  Matrix ret(ma.m_nrow, mb.m_ncol);
  for (size_t r = 0; r < ma.m_nrow; ++r) {
    for (size_t c = 0; c < mb.m_ncol; ++c) {
      double tmp = 0;
      for (size_t i = 0; i < ma.m_ncol; ++i) {
        tmp += ma(r, i) * mb(i, c);
      }
      ret(r, c) = tmp;
    }
  }
  return ret;
}

Matrix multiply_tile(const Matrix &ma, const Matrix &mb, const size_t t_size) {
  // t_size : size of tile
  if (ma.m_ncol != mb.m_nrow) {
    throw py::value_error("Matrices cannot be multiplied!");
  }
  Matrix ret(ma.m_nrow, mb.m_ncol);
  for (size_t r = 0; r < ma.m_nrow; ++r) {
    for (size_t c = 0; c < mb.m_ncol; ++c) {
      ret(r, c) = 0;
    }
  }
  for (size_t row_L = 0; row_L < ma.m_nrow; row_L += t_size) {
    size_t row_R = std::min(row_L + t_size, ma.m_nrow);
    for (size_t col_L = 0; col_L < mb.m_ncol; col_L += t_size) {
      size_t col_R = std::min(col_L + t_size, mb.m_ncol);
      for (size_t i_L = 0; i_L < ma.m_ncol; i_L += t_size) {
        size_t i_R = std::min(i_L + t_size, ma.m_ncol);
        for (int i = i_L; i < i_R; ++i) {
          for (size_t r = row_L; r < row_R; ++r) {
            const double ri = ma(r, i);
            for (size_t c = col_L; c < col_R; ++c) {
              ret(r, c) += ri * mb(i, c);
            }
          }
        }
      }
    }
  }
  return ret;
}

Matrix multiply_mkl(const Matrix &ma, const Matrix &mb) {
  if (ma.m_ncol != mb.m_nrow) {
    throw py::value_error("Matrices cannot be multiplied!");
  }
  Matrix ret(ma.m_nrow, mb.m_ncol);
  mkl_set_num_threads(1);
  const double alpha = 1.0;
  const double beta = 0.0;
  // C = alpha * A * B + beta * C
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, ma.m_nrow, mb.m_ncol,
              ma.m_ncol, alpha, ma.m_buffer, ma.m_ncol, mb.m_buffer, mb.m_ncol,
              beta, ret.m_buffer, ret.m_ncol);
  return ret;
}

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "matrix multiplication";
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);
  py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def(py::init<std::vector<std::vector<double>> &>())
      .def_readwrite("nrow", &Matrix::m_nrow)
      .def_readwrite("ncol", &Matrix::m_ncol)
      .def(
          "__getitem__",
          [](Matrix &mat, std::pair<size_t, size_t> p) {
            return mat(p.first, p.second);
          },
          py::is_operator())
      .def(
          "__setitem__",
          [](Matrix &mat, std::pair<size_t, size_t> p, double val) {
            mat(p.first, p.second) = val;
          },
          py::is_operator())
      .def("__eq__", &Matrix::operator==)
      .def(py::self==py::self);
}
