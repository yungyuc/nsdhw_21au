#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <tuple>

class Matrix{
public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol){
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol){
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec){
        if (size() != vec.size()){
            throw std::out_of_range("number of elements mismatch");
        }
        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i){
            for (size_t j=0; j<m_ncol; ++j){
                (*this)(i,j) = vec[k];
                ++k;
            }
        }
        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
      , m_elapsed(other.m_elapsed), m_nflo(other.m_nflo){
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix & operator=(Matrix const & other){
        if (this == &other) { return *this; }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        m_elapsed = other.m_elapsed;
        m_nflo = other.m_nflo;
        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
      , m_elapsed(other.m_elapsed), m_nflo(other.m_nflo){
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other){
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        std::swap(m_elapsed, other.m_elapsed);
        std::swap(m_nflo, other.m_nflo);
        return *this;
    }

    ~Matrix(){
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

    double   elapsed() const { return m_elapsed; }
    double & elapsed()       { return m_elapsed; }

    size_t   nflo() const { return m_nflo; }
    size_t & nflo()       { return m_nflo; }

    double gflops() const { return m_nflo / m_elapsed / 1.e9; }


    void print(){
        for(size_t i=0; i<m_nrow; i++){
            for(size_t j=0; j<m_ncol; j++){
                std::cout << std::setw(3) << (*this)(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    size_t index(size_t row, size_t col) const{
        return row * m_ncol + col;
    }

    double getitem(std::tuple<size_t, size_t> rowcol){
        return m_buffer[std::get<0>(rowcol)*m_nrow + std::get<1>(rowcol)];
        // return (*this)(std::get<0>(rowcol), std::get<1>(rowcol));
    }

    void setitem(std::tuple<size_t, size_t> rowcol, double value){
        m_buffer[std::get<0>(rowcol)*m_nrow + std::get<1>(rowcol)] = value;
    }

    void reset_buffer(size_t nrow, size_t ncol){
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;
    double m_elapsed = 0;
    size_t m_nflo = 0; // number of floating-point operations.

};