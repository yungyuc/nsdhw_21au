#pragma once
#include <cstddef>

class Matrix
{
public:
    Matrix();                               // default construtor
    Matrix(Matrix const & );                // copy constructor
    Matrix(Matrix       &&);                // move constructor
    Matrix & operator=(Matrix const & );    // copy assignment operator
    Matrix & operator=(Matrix       &&);    // move assignment operator
    Matrix(size_t nrow, size_t ncol);       // custom constructor
    ~Matrix();                              // destructor
    size_t nrow() const;                    // access the number of rows
    size_t ncol() const;                    // access the number of columns
    double* buff() const;                   // access the raw contents of the buffer
    double   operator() (size_t row, size_t col) const; // element getter
    double & operator() (size_t row, size_t col);       // element setter

private:
    size_t m_nrow;
    size_t m_ncol;
    double *m_buffer;
}; /* end of class Matrix */

// Free functions for multiplication
Matrix multiply_naive(Matrix const &matA, Matrix const &matB);
Matrix multiply_tile(Matrix const &matA, Matrix const &matB, size_t const tile_size);
Matrix multiply_mkl(Matrix const &matA, Matrix const &matB);

