#include "Mat.hpp"
#include <iostream>
#include <ctime>

int main() {
  using namespace std;
  Matrix A(1000, 2000);
  Matrix B(2000, 3000);

  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 20; ++j) {
      A.getpos(i, j) = 5;
    }
  }

  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < 30; ++j) {
      B.getpos(i, j) = 5;
    }
  }

  time_t cur_time;
  time_t fin_time;
  time(&cur_time);
  Matrix C = multiply_naive(A, B);
  time(&fin_time);
  cout<< (fin_time - cur_time)<<endl;


  //cout << C << endl;
  time(&cur_time);
  C = multiply_tile(A,B);
  time(&fin_time);
  cout<< (fin_time - cur_time)<<endl;
  //cout << C << endl;
  return 0;
}
