#include <iostream>
#include "Mat.hpp"


int main(){
  using namespace std;
  Matrix A(10,20);
  Matrix B(20,30);

  for(int i=0; i<10; ++i){
    for(int j=0; j<20; ++j){
      A.getpos(i,j) = 5;
    }
  }

  for(int i=0; i<20; ++i){
    for(int j=0; j<30; ++j){
      B.getpos(i,j) = 5;
    }
  }

  Matrix C = A.NaiveMatmul(&B);
  cout<<C<<endl;
  //for(int i=0; i<10;++i){
  //  for(int j=0; j<30; ++j){
  //    cout<<C.getdata(i,j)<<" ";
  //  }
  //  cout<<endl;
  //}
  return 0;
}
