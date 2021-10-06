#include <pybind11/pybind11.h>
#include <cmath>
#define square(x) ((x)*(x))

std::pair<double,double> create_vector(double x, double y){
    return std::make_pair(x,y);
}

double calc(std::pair<double,double> src1,std::pair<double,double> src2) {
    if((src1.first==0.0 && src1.second==0.0)||(src2.first==0.0 && src2.second==0.0)){
        throw std::length_error("zero-length 2-vector (invalid input)");
    }
    double vector_mul = src1.first*src2.first+src1.second*src2.second;
    double src1_len = pow(square(src1.first)+square(src1.second),0.5);
    double src2_len = pow(square(src2.first)+square(src2.second),0.5);
    return acos(vector_mul/(src1_len*src2_len));
}

// module name: _vector, according to validate.sh
PYBIND11_MODULE(_vector, m) {
    m.doc() = "calculates the angle (in radians) between two vectors \
                in the 2-dimensional Cartesian coordinate system"; 
    m.def("calc", &calc, "a function calculates the angle");
    m.def("create_vector", &create_vector, "a function to create vector");
}