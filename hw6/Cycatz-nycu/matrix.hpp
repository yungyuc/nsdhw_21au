#include <string>
#include <utility>
#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

class Matrix {

public:

    Matrix() : m_nrow(0), m_ncol(0), m_elems(nullptr) {} // default constructor 
    Matrix(const Matrix &rhs);                           // copy constructor
    // Matrix(Matrix &&);                                // move constructor
    Matrix& operator=(const Matrix &rhs);                // copy assignment operator
    // Matrix & operator=(Matrix &&);                    // move assignment operator
    Matrix(size_t r, size_t c) : m_nrow(r), m_ncol(c) {  // custom constructor
        if (r == 0 || c == 0) {
            throw std::out_of_range("matrix index out of range");
        }
        m_elems = new double[r * c];
        for (size_t i = 0; i < r; i++) {
            for (size_t j = 0; j < c; j++) {
                m_elems[i * c + j] = 0.0;
            }
        }
    }

    ~Matrix() { delete[] m_elems; }

    pybind11::array_t<double> ndarray();

    double& operator() (size_t i, size_t j) {
        /*
            Remove this check to speed up the performance
            if (i >= m_nrow || j >= m_ncol)
                throw std::out_of_range("matrix index out of range");
        */
        return m_elems[i * m_ncol + j];
    }

    double operator() (size_t i, size_t j) const {
        /*
            Remove this check to speed up the performance
            if (i >= m_nrow || j >= m_ncol)
                throw std::out_of_range("matrix index out of range");
        */
        return m_elems[i * m_ncol + j];
    }


    std::pair<size_t, size_t> size() const;
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_nrow; }

    bool operator== (const Matrix &rhs) const; 

    std::string to_string() const;

    /* For accessing the inner buffer */
    friend Matrix multiply_mkl(const Matrix &A, const Matrix &B);

private:
    bool check_range(size_t i, size_t j) const;
    size_t m_nrow, m_ncol;
    double *m_elems;

};

Matrix multiply_naive(const Matrix &A, const Matrix &B);
Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_width); 
Matrix multiply_mkl(const Matrix &A, const Matrix &B);
