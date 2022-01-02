#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <cstring>
#include <vector>

class Matrix {

  public:
    // Custom constructor
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
      reset_buffer(nrow, ncol);
    }
    Matrix(const Matrix &m)
      : m_nrow(m.m_nrow), m_ncol(m.m_ncol){
        reset_buffer(m_nrow, m_ncol);
        size_t nelement = m_nrow * m_ncol;
        memcpy(m_buffer, m.m_buffer, nelement*sizeof(double));
      }

    Matrix(Matrix &&other)
      : m_nrow{other.m_nrow}, m_ncol{other.m_ncol}{
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
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

    ////Ref: https://stackoverflow.com/questions/44659924/returning-numpy-arrays-via-pybind11
    py::array_t<double> array() {
          return py::array_t<double>(
          {nrow(), ncol()}, // shape
          {sizeof(double) * ncol(), sizeof(double)}, // C-style contiguous strides for ncol doubles
          m_buffer, // the data pointer
          py::cast(this)); // numpy array references this parent
    }


    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }

    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tile_size);
    friend Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);

  private:
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;

};

bool operator==(const Matrix &mat1, const Matrix &mat2);

void validate_multiplication(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tile_size);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);
