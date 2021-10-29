#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <tuple>

class Matrix{
public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol){
        if (size()) { m_buffer = new double[size()]; }
        else { m_buffer = nullptr; }
        for (size_t i = 0; i < size(); ++i)
            m_buffer[i] = 0;
    }

    // ~Matrix(){
    //     delete [] m_buffer;
    // }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }

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
    }

    void setitem(std::tuple<size_t, size_t> rowcol, double value){
        m_buffer[std::get<0>(rowcol)*m_nrow + std::get<1>(rowcol)] = value;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

};