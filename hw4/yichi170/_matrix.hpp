#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>
#include "allocator.hpp"

class Matrix {
public:
    Matrix(size_t, size_t);
    Matrix(const Matrix &);
    Matrix(Matrix &&);
    Matrix & operator=(const Matrix &);
    ~Matrix();

    bool operator== (Matrix const & mt) const;

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }
    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t index(size_t row, size_t col) const { return row * m_ncol + col; }

    void reset_buffer(size_t nrow, size_t ncol) {
        const size_t nelement = nrow * ncol;
        m_buffer.resize(nelement, 0);
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, CustomAllocator<double>> m_buffer;
    // double * m_buffer = nullptr;
};

Matrix multiply_naive(const Matrix &, const Matrix &);
Matrix multiply_tile(const Matrix &, const Matrix &, size_t);
Matrix multiply_ntile(const Matrix &, const Matrix &, size_t);
Matrix multiply_mkl(const Matrix &, const Matrix &);

std::size_t bytes();
std::size_t allocated();
std::size_t deallocated();

#endif