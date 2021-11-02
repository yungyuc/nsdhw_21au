#include <string>
#include <utility>
#include <cstddef>
#include <algorithm>

class Matrix {
public:

    Matrix() : m_nrow(0), m_ncol(0), m_elems(nullptr) {} // default constructor 
    Matrix(const Matrix &rhs);                           // copy constructor
    // Matrix(Matrix &&);                                // move constructor
    Matrix& operator=(const Matrix &rhs);                // copy assignment operator
    // Matrix & operator=(Matrix &&);                    // move assignment operator
    Matrix(size_t r, size_t c);                          // custom constructor

    ~Matrix() { delete[] m_elems; }

    double& operator() (size_t i, size_t j);
    double operator() (size_t i, size_t j) const;


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
