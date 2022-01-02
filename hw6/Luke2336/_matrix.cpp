#include <atomic>
#include <mkl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

namespace py = pybind11;

struct ByteCounterImpl {
  std::atomic_size_t allocated = 0;
  std::atomic_size_t deallocated = 0;
  std::atomic_size_t refcount = 0;
};

class ByteCounter {

public:
  ByteCounter() : m_impl(new ByteCounterImpl) { incref(); }

  ByteCounter(ByteCounter const &other) : m_impl(other.m_impl) { incref(); }

  ByteCounter &operator=(ByteCounter const &other) {
    if (&other != this) {
      decref();
      m_impl = other.m_impl;
      incref();
    }

    return *this;
  }

  ByteCounter(ByteCounter &&other) : m_impl(other.m_impl) { incref(); }

  ByteCounter &operator=(ByteCounter &&other) {
    if (&other != this) {
      decref();
      m_impl = other.m_impl;
      incref();
    }

    return *this;
  }

  ~ByteCounter() { decref(); }

  void swap(ByteCounter &other) { std::swap(m_impl, other.m_impl); }

  void increase(std::size_t amount) { m_impl->allocated += amount; }

  void decrease(std::size_t amount) { m_impl->deallocated += amount; }

  std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
  std::size_t allocated() const { return m_impl->allocated; }
  std::size_t deallocated() const { return m_impl->deallocated; }
  /* This is for debugging. */
  std::size_t refcount() const { return m_impl->refcount; }

private:
  void incref() { ++m_impl->refcount; }

  void decref() {
    if (nullptr == m_impl) {
      // Do nothing.
    } else if (1 == m_impl->refcount) {
      delete m_impl;
      m_impl = nullptr;
    } else {
      --m_impl->refcount;
    }
  }

  ByteCounterImpl *m_impl;
};

template <class T> struct CustomAllocator {

  using value_type = T;

  CustomAllocator() = default;

  template <class U>
  constexpr CustomAllocator(const CustomAllocator<U> &other) noexcept {
    counter = other.counter;
  }

  T *allocate(std::size_t n) {
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
      throw std::bad_alloc();
    }
    const std::size_t bytes = n * sizeof(T);
    T *p = static_cast<T *>(std::malloc(bytes));
    if (p) {
      counter.increase(bytes);
      return p;
    } else {
      throw std::bad_alloc();
    }
  }

  void deallocate(T *p, std::size_t n) noexcept {
    std::free(p);
    const std::size_t bytes = n * sizeof(T);
    counter.decrease(bytes);
  }

  ByteCounter counter;
};

static CustomAllocator<double> allocator;

struct Matrix {
  size_t nrow = 0;
  size_t ncol = 0;
  std::vector<double, CustomAllocator<double>> buffer;
  Matrix(size_t nrow, size_t ncol) : nrow(nrow), ncol(ncol), buffer(allocator) {
    reset_buffer(nrow, ncol);
  }
  Matrix(Matrix &&other)
      : nrow(other.nrow), ncol(other.ncol), buffer(allocator) {
    reset_buffer(0, 0);
    std::swap(nrow, other.nrow);
    std::swap(ncol, other.ncol);
    std::swap(buffer, other.buffer);
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

  py::array_t<double> array() {
    return py::array_t<double>({nrow, ncol},
                               {sizeof(double) * ncol, sizeof(double)},
                               buffer.data(), py::cast(this));
  }
};

Matrix multiply_naive(const Matrix &ma, const Matrix &mb) {
  if (ma.ncol != mb.nrow) {
    throw py::value_error("Matrices cannot be multiplied!");
  }
  Matrix ret(ma.nrow, mb.ncol);
  for (size_t r = 0; r < ma.nrow; ++r) {
    for (size_t c = 0; c < mb.ncol; ++c) {
      double tmp = 0;
      for (size_t i = 0; i < ma.ncol; ++i) {
        tmp += ma(r, i) * mb(i, c);
      }
      ret(r, c) = tmp;
    }
  }
  return ret;
}

Matrix multiply_mkl(const Matrix &ma, const Matrix &mb) {
  if (ma.ncol != mb.nrow) {
    throw py::value_error("Matrices cannot be multiplied!");
  }
  Matrix ret(ma.nrow, mb.ncol);
  mkl_set_num_threads(1);
  const double alpha = 1.0;
  const double beta = 0.0;
  // C = alpha * A * B + beta * C
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, ma.nrow, mb.ncol,
              ma.ncol, alpha, ma.buffer.data(), ma.ncol, mb.buffer.data(),
              mb.ncol, beta, ret.buffer.data(), ret.ncol);
  return ret;
}

size_t bytes() { return allocator.counter.bytes(); }
size_t allocated() { return allocator.counter.allocated(); }
size_t deallocated() { return allocator.counter.deallocated(); }

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "matrix memory management";
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_mkl", &multiply_mkl);
  m.def("bytes", &bytes);
  m.def("allocated", &allocated);
  m.def("deallocated", &deallocated);
  py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def_readwrite("nrow", &Matrix::nrow)
      .def_readwrite("ncol", &Matrix::ncol)
      .def_property("array", &Matrix::array, nullptr)
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
      .def(py::self == py::self);
}