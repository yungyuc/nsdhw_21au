#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;
namespace py = pybind11;

bool testZeroLength(vector<double>&);

double getAngle(vector<double> &a, vector<double> &b) {
    if (a.size() != b.size())
        return -1;

    if (testZeroLength(a) || testZeroLength(b))
        throw std::invalid_argument("zero length vector!");
    
    long double len_a = 0;
    long double len_b = 0;
    for (auto e : a)
        len_a += e*(long double)e;
    for (auto e : b)
        len_b += e*(long double)e;
    len_a = (double)sqrt(len_a);
    len_b = (double)sqrt(len_b);

    long double dot_prod = 0;
    for (size_t i=0; i<a.size(); ++i)
        dot_prod += a[i]*b[i];

    return acos(dot_prod/(len_a*len_b));
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "pybind11 example plugin";      // module doc string
  m.def("getAngle",                              // function name
        &getAngle,                               // function pointer
        "A function which calculates \
        the angle (in radians) between two vectors" //function doc string
       );
}

bool testZeroLength(vector<double>& vec){
    for (auto e : vec)
        if (e != 0.0)
            return false;
    return true;
}