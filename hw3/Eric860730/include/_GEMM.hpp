#pragma once
#include <cstring>

class Matrix {

public:
    // Custom constructor
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
      size_t nelement = nrow * ncol;
      m_buffer = new double[nelement]();
    }
    Matrix(const Matrix &m){
      m_nrow = m.m_nrow;
      m_ncol = m.m_ncol;
      size_t nelement = m_nrow * m_ncol;
      m_buffer = new double[nelement]();
      memcpy(m_buffer, m.m_buffer, nelement*sizeof(double));
    }
    // Destructor
    ~Matrix(){
      delete []m_buffer;
    }

    // No bound check.
    double  operator() (size_t row, size_t col) const{ return m_buffer[row*m_ncol + col]; } // getter (the value of Matrix[row][col])
    double &operator() (size_t row, size_t col)      { return m_buffer[row*m_ncol + col]; } // setter (the value of Matrix[row][col])
    bool operator==(const Matrix &other) const
    {
      if( nrow() != other.nrow() || ncol() != other.ncol() ){
        return false;
      }

      size_t idx_base=0;
      for( size_t i = 0; i<m_nrow; i++){
        for(size_t j=0; j<m_ncol; j++){
          size_t idx = idx_base+j;
          if(m_buffer[idx] != other.m_buffer[idx]){
            return false;
          }
        }
        idx_base += m_ncol;
      }
      return true;
    }


    double * raw_data() const { return m_buffer;}
    const size_t nrow() const { return m_nrow; }
    const size_t ncol() const { return m_ncol; }

private:

    size_t m_nrow;
    size_t m_ncol;
    double * m_buffer;

};

void validate_multiplication(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tile_size);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);
