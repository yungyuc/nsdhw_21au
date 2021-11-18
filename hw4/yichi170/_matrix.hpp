#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>
#include "allocator.hpp"

static CustomAllocator<double> alloc;

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol), m_buffer(alloc) {
        reset_buffer(nrow, ncol);
    }

    Matrix(const Matrix & other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
        
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i)
            for (size_t j = 0; j < m_ncol; ++j)
                (*this)(i, j) = other(i, j);
    }

    Matrix & operator=(const Matrix & other) {
        
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
            reset_buffer(other.m_nrow, other.m_ncol);
        
        for (size_t i = 0; i < m_nrow; i++)
            for (size_t j = 0; j < m_ncol; j++)
                (*this)(i, j) = other(i, j);
            
        return *this;
    }

    ~Matrix() {  reset_buffer(0, 0); }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    
    bool operator== (Matrix const & mt) const {
        if ((m_ncol != mt.m_ncol) && (m_nrow != mt.m_nrow))
            return false;

        for (size_t i = 0; i < m_nrow; i++)
            for (size_t j = 0; j < m_ncol; j++)
                if ((*this)(i, j) != mt(i, j))
                    return false;
        
        return true;
    }

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

#endif