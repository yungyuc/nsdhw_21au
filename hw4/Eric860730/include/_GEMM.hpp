#pragma once
#include <cstring>
#include <vector>

#include "MyAllocator.hpp"

static MyAllocator<double> matrix_allocator;

class Matrix {

  public:
    // Custom constructor
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(matrix_allocator)
    {
      reset_buffer(nrow, ncol);
    }
    Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(matrix_allocator) {
        reset_buffer(nrow, ncol);
        (*this) = vec;
      }
    Matrix(const Matrix &m)
      : m_nrow(m.m_nrow), m_ncol(m.m_ncol), m_buffer(matrix_allocator) {
        reset_buffer(m_nrow, m_ncol);
        for (size_t i = 0; i < m_nrow; ++i) {
          for (size_t j = 0; j < m_ncol; ++j) {
            (*this)(i, j) = m(i, j);
          }
        }
      }

    Matrix(Matrix &&other)
      : m_nrow{other.m_nrow}, m_ncol{other.m_ncol}, m_buffer{matrix_allocator} {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
      }

    Matrix &operator=(std::vector<double> const &vec) {
      if (size() != vec.size()) {
        throw std::out_of_range("different vector size.");
      }

      size_t index = 0;
      for (size_t i = 0; i < m_nrow; ++i) {
        for (size_t j = 0; j < m_ncol; ++j) {
          (*this)(i, j) = vec[index];
          ++index;
        }
      }
      return *this;
    }

    Matrix &operator=(Matrix const &other) {
      if (this == &other) {
        return *this;
      }
      // check shape
      if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
        reset_buffer(other.m_nrow, other.m_ncol);
      }
      for (size_t i = 0; i < m_nrow; ++i) {
        for (size_t j = 0; j < m_ncol; ++j) {
          (*this)(i, j) = other(i, j);
        }
      }
      return *this;
    }

    Matrix &operator=(double val) {
      for (size_t i = 0, end = m_nrow * m_ncol; i < end; ++i) {
        m_buffer[i] = val;
      }
      return *this;
    }
    // Destructor
    ~Matrix(){
      reset_buffer(0, 0);
    }

    // No bound check.
    double  operator() (size_t row, size_t col) const{ return m_buffer[row * m_ncol + col]; } // getter (the value of Matrix[row][col])
    double &operator() (size_t row, size_t col)      { return m_buffer[row * m_ncol + col]; } // setter (the value of Matrix[row][col])

    void reset_buffer(size_t nrow, size_t ncol) {
      const size_t nelement = nrow * ncol;
      m_buffer.reserve(nelement);
      m_nrow = nrow;
      m_ncol = ncol;
    }


    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }

    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tile_size);
    friend Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);

  private:
    size_t m_nrow;
    size_t m_ncol;
    std::vector<double, MyAllocator<double>> m_buffer;

};

bool operator==(const Matrix &mat1, const Matrix &mat2);
std::size_t bytes();
std::size_t allocated();
std::size_t deallocated();

void validate_multiplication(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tile_size);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);
