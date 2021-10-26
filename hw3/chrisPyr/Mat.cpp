#include "Mat.hpp"

Matrix Matrix::NaiveMatmul(Matrix  *other){
  Matrix res(this->getrow(), other->getcol());
  for(size_t i=0; i<this->getrow(); ++i){
    for(size_t j=0; j<other->getcol(); ++j){
      double result = 0;
      for(size_t k=0; k<this->getcol(); ++k){
        result += this->getdata(i,k) * other->getdata(k,j);
      }
      res.getpos(i,j) = result;
    }
  }
  return res;
}

