#ifndef _MATRIX_H
#define _MATRIX_H
#include <cstddef>
#include <limits>
#include <new>
#include <string>
#include <vector>
#include <iostream>
#include <atomic>

using std::vector;

struct Count {
  std::atomic_size_t allocated;
  std::atomic_size_t deallocated;
  std::atomic_size_t refcount;
  Count() : allocated(0), deallocated(0), refcount(0) {}
};

class ByteCounter {
public:
  ByteCounter() : count(new Count()) { addref(); }
  ByteCounter(const ByteCounter &other) : count(other.count) {
    addref();
  }
  ByteCounter& operator=(const ByteCounter &other) {
    if (&other != this) {
      delref();
      count = other.count;
      addref();
    }
    return *this;
  }
  ByteCounter(ByteCounter &&other) : count(other.count) {
    addref();
  }
  ByteCounter& operator=(ByteCounter &&other) {
    if (&other != this) {
      delref();
      count = other.count;
      addref();
    }
    return *this;
  }

  ~ByteCounter() { delref(); }

  void swap(ByteCounter &other) {
    std::swap(count, other.count);
  }

  void increase(std::size_t amount) {
    count->allocated += amount;
  }

  void decrease(std::size_t amount) {
    count->deallocated += amount;
  }

  std::size_t bytes() const { return count->allocated - count->deallocated; }
  std::size_t allocated() const { return count->allocated; }
  std::size_t deallocated() const { return count->deallocated; }
  
private:
  Count *count;
  
  void addref() { ++count->refcount; }
  
  void delref() {
    if (count != nullptr) {
      if (count->refcount == 1) {
	delete count;
	count = nullptr;
      } else {
	--count->refcount;
      }
    }
  }
};

template <class T>
struct CustomAllocator {
  typedef T value_type;

  CustomAllocator() = default;

  template<class U> constexpr
  CustomAllocator(const CustomAllocator<U> &other) noexcept : counter(other.counter) {}

  T *allocate(std::size_t n) {
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
      throw std::bad_array_new_length();
    }
    const std::size_t bytes = n * sizeof(T);
    T *p = static_cast<T*>(std::malloc(bytes));
    if (p) {
      counter.increase(bytes);
    } else {
      throw std::bad_alloc();
    }
    return p;
  }

  void deallocate(T* p, std::size_t n) noexcept {
    std::free(p);
    const std::size_t bytes = n * sizeof(T);
    counter.decrease(bytes);
  }

  std::size_t bytes() const { return counter.bytes(); }
  std::size_t allocated() const { return counter.allocated(); }
  std::size_t deallocated() const { return counter.deallocated(); }
  ByteCounter counter;
};


class Matrix {
 public:
  size_t nrow, ncol;
  Matrix(size_t row, size_t col);
  Matrix() : Matrix(0, 0) {}
  // Matrix(size_t row, size_t col, std::vector<double> const & vec);
  Matrix(const Matrix &m);
  Matrix& operator=(const Matrix &m);
  Matrix(Matrix &&m);
  ~Matrix() = default;
  
  double& operator() (size_t i, size_t j) {
    return this->mat[i * (this->ncol) + j];
  }

  double operator() (size_t i, size_t j) const {
    return this->mat[i * (this->ncol) + j];
  }
  
  bool operator== (const Matrix& B) const;
  std::string to_string() const;
  friend Matrix multiply_mkl(const Matrix& A, const Matrix&B);
 private:
  vector<double, CustomAllocator<double> > mat;
  void init(size_t nrow, size_t ncol);
};

Matrix multiply_naive(const Matrix& A, const Matrix& B);
Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_width);
Matrix multiply_mkl(const Matrix &A, const Matrix &B);

#endif
