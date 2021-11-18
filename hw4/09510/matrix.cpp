#include <algorithm>
#include "mkl.h"
#include <cstddef>
#include <atomic>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace pybind = pybind11;
using namespace std;


struct ByteCounterImpl
{

    std::atomic_size_t allocated {0};
    std::atomic_size_t deallocated {0};
    std::atomic_size_t refcount {0};

}; /* end struct ByteCounterImpl */

/**
 * One instance of this counter is shared among a set of allocators.
 *
 * The counter keeps track of the bytes allocated and deallocated, and report
 * those two numbers in addition to bytes that remain allocated.
 */
class ByteCounter
{

public:

    ByteCounter()
      : m_impl(new ByteCounterImpl)
    { incref(); }

    ByteCounter(ByteCounter const & other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter const & other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter && other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter && other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ~ByteCounter() { decref(); }

    void swap(ByteCounter & other)
    {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount)
    {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount)
    {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }
    /* This is for debugging. */
    std::size_t refcount() const { return m_impl->refcount; }

private:

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



/**
 * Very simple allocator that counts the number of bytes allocated through it.
 *
 * It's made to demonstrate the STL allocator and only works in this example.
 * A lot of modification is needed to use it in a real application.
 */
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




static MyAllocator<double> allocate;
class Matrix {
  public:
    Matrix(size_t nrow, size_t ncol): m_nrow(nrow), m_ncol(ncol), m_buffer(allocate) {}
    Matrix(const vector<vector<double>> &m)
    {
      m_nrow = m.size();
      m_ncol = m[0].size();
      for (auto &i: m) {
        m_buffer.insert(end(m_buffer), begin(i), end(i));
      }
    }

    bool operator==(const Matrix &other)
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
    vector<double,MyAllocator<double> > m_buffer;
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


size_t bytes() { return allocate.counter.bytes(); }
size_t allocated() { return allocate.counter.allocated(); }
size_t deallocated() { return allocate.counter.deallocated(); }

PYBIND11_MODULE(_matrix, m) {
  
  m.doc() = "Matrix multiply";
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);
  m.def("bytes", &bytes);
  m.def("allocated", &allocated);
  m.def("deallocated", &deallocated);

  pybind::class_<Matrix>(m, "Matrix")
      .def(pybind::init<size_t, size_t>())
      .def(pybind::init<const std::vector<std::vector<double>> &>())

      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)

      .def("__eq__", &Matrix::operator==)
      .def("__getitem__",[](const Matrix &m, array<int, 2> idx) { return m(idx[0], idx[1]); })
      .def("__setitem__", [](Matrix &m, array<int, 2> idx, double val) {m(idx[0], idx[1]) = val;});
}