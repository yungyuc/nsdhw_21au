#ifndef MAT_HPP_
#define MAT_HPP_

#include <stddef.h>
#include <utility>
#include <iostream>

class Matrix{
  private:
    size_t m_row, m_col;
    double *data;
  public:
    Matrix(size_t row, size_t col): m_row{row}, m_col{col}{
      data = new double[m_row * m_col];
      // data initialize
      for(size_t i=0; i<row; ++i){
        for(size_t j=0; j<col; ++j){
          data[i*m_col+j] = 0.0f;
        }
      }
    };

    ~Matrix(){
      delete data;
    };

    Matrix(Matrix &other){
      this->m_row = other.m_row;
      this->m_col = other.m_col;
      for(size_t i=0; i<m_row; ++i){
        for(size_t j=0; j<m_col; ++j){
          this->getpos(i,j) = other.getdata(i,j);
        }
      }
    }

    Matrix &operator=(Matrix &&other){

      std::swap(m_row, other.m_row);
      std::swap(m_col, other.m_col);
      std::swap(data, other.data);
      return *this;
    }

friend std::ostream& operator<<(std::ostream &os, Matrix &mat){
      for(size_t i=0; i<mat.m_row; ++i){
        for(size_t j=0; j<mat.m_col; ++j){
          os<<mat.getdata(i,j)<<' ';
        }
        os<<std::endl;
      }
      return os;
    }

    size_t getrow(){  return m_row; }
    size_t getcol(){  return m_col; }
    double getdata(size_t row, size_t col){
      return data[row*m_col+col];
    }
    double &getpos(size_t row, size_t col){
      return data[row*m_col+col];
    }
    Matrix NaiveMatmul(Matrix *other);
    Matrix TilingMatmul(Matrix *other);

};

#endif
