#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cmath>

namespace py = pybind11;
using namespace std;

bool testZeroLength(const vector<double>& vec);
double getAngle(const std::vector<double> vec1, const std::vector<double> vec2);

double getAngle(const std::vector<double> vec1, const std::vector<double> vec2){
    if (testZeroLength(vec1) || testZeroLength(vec2)) throw std::invalid_argument("Vector with zero length.");

    double vec1_len = (double) sqrt(vec1[0]*vec1[0]+vec1[1]*vec1[1]);
    double vec2_len = (double) sqrt(vec2[0]*vec2[0]+vec2[1]*vec2[1]);
    double inner_prod = vec1[0]*vec2[0]+vec1[1]*vec2[1];
    
    return acos(inner_prod/(vec1_len*vec2_len));
}

bool testZeroLength(const vector<double>& vec){
    for (auto val:vec)
        if (val != 0.0) return false;
    return true;
}

PYBIND11_MODULE(_vector,m){
    m.doc() = "Pybind11 simple test";
    m.def("getAngle",&getAngle,"Function that calculates\
     the the angle between two vectors in radian expression.");
}