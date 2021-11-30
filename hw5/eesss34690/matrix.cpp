    void incref() { ++m_impl->refcount; }

    void decref()
    {
        if (nullptr == m_impl)
        {
            // Do nothing.
        }
        else if (1 == m_impl->refcount)
        {
            delete m_impl;
            m_impl = nullptr;
        }
        else
        {
            --m_impl->refcount;
        }
    }

    ByteCounterImpl * m_impl;

}; /* end class ByteCounter */

template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator(const MyAllocator<U> & other) noexcept
    {
        counter = other.counter;
    }

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            counter.increase(bytes);
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* end struct MyAllocator */

template <class T, class U>
bool operator==(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return a.counter == b.counter;
}

template <class T, class U>
bool operator!=(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return !(a == b);
}

static MyAllocator<double> alloc;

class Matrix {
 public:
  Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(alloc)
  {
  	m_buffer.resize(m_nrow* m_ncol, 0.0);
  }
  Matrix(const vector<vector<double>> &m);

  bool operator==(const Matrix &other);
  double operator()(size_t row, size_t col) const {
    return m_buffer[row * m_ncol + col];
  }
  double &operator()(size_t row, size_t col) {
    return m_buffer[row * m_ncol + col];
  }

  size_t nrow() const { return m_nrow; }
  size_t ncol() const { return m_ncol; }
  double *buffer() { return m_buffer.data(); }
  const double *buffer() const { return m_buffer.data(); }

 private:
  size_t m_nrow;
  size_t m_ncol;
  vector<double, MyAllocator<double>> m_buffer;
};

Matrix::Matrix(const vector<vector<double>> &m) {
  m_nrow = m.size();
  m_ncol = m[0].size();
  //m_buffer = alloc;
  for (auto &i: m) {
    m_buffer.insert(end(m_buffer), begin(i), end(i));
  }
}

bool Matrix::operator==(const Matrix &other) {
  if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
    return false;
  }
  for (size_t i = 0; i < m_nrow; ++i) {
    for (size_t j = 0; j < m_ncol; ++j) {
      if ((*this)(i, j) != other(i, j)) {
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

size_t bytes() { return alloc.counter.bytes(); }
size_t allocated() { return alloc.counter.allocated(); }
size_t deallocated() { return alloc.counter.deallocated(); }

PYBIND11_MODULE(_matrix, m) {
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);
  m.def("bytes", &bytes);
  m.def("allocated", &allocated);
  m.def("deallocated", &deallocated);

  py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def(py::init<const std::vector<std::vector<double>> &>())
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)
      .def("__eq__", &Matrix::operator==)
      .def("__getitem__",
           [](const Matrix &m, array<int, 2> idx) { return m(idx[0], idx[1]); })
      .def("__setitem__", [](Matrix &m, array<int, 2> idx, double val) {
        m(idx[0], idx[1]) = val;
      });
}