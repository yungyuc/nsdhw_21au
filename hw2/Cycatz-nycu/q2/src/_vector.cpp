#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cmath>


double calc(const std::vector<double> &v1, const std::vector<double> &v2)
{
    if (v1.size() != 2 || v2.size() != 2) {
        throw std::length_error("The dimension of vectors must be 2");
    }

    // Method taken from https://www.jwwalker.com/pages/angle-between-vectors.html  
    double dot_product = v1.at(0) * v2.at(0) + v1.at(1) * v2.at(1);  
    double cross_product = v1.at(0) * v2.at(1) - v1.at(1) * v2.at(0);    

    return atan2(cross_product, dot_product); 
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "A module which calculates the angle between two vectors";
    m.def("calc", &calc, "A function which calculates the angle between two vectors");
}

