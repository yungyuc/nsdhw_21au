#include <iostream>
#include <pybind11/pybind11.h>
#include <math.h>

double calc_angle(std::pair<double, double> v1, std::pair<double, double> v2){
    double length_1 = sqrt(v1.first*v1.first + v1.second*v1.second);
    double length_2 = sqrt(v2.first*v2.first + v2.second*v2.second);

    if (length_1 == 0.0 || length_2 == 0.0)
        throw std::invalid_argument("Error: zero length");
        
    double unit_dot = (v1.first*v2.first + v1.second*v2.second) / (length_1 * length_2);
    return acos(unit_dot);
}

PYBIND11_MODULE(_vector, m){
    m.def("calc_angle", &calc_angle, "Calculates the angle (in radians) between two vectors.");
}