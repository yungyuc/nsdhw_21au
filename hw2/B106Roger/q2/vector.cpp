#include <pybind11/pybind11.h>
#include <iostream>
#include <math.h>
namespace py = pybind11;

float angle(float i1, float j1, float i2, float j2) {
    float dot_value = i1*i2+j1*j2;
    float v1_len = sqrt(i1*i1+j1*j1);
    float v2_len = sqrt(i2*i2+j2*j2);
    if (v1_len == 0.0 || v2_len == 0.0)
        return 0.0;
    float cos_value = dot_value / v1_len / v2_len;
    if (cos_value >= 1.0)
        return 0.0;
    else if (cos_value <= -1.0)
        return M_PI;
    else 
        return acos(cos_value);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "nsd21au hw2 q2 pybind implementation"; // optional module docstring
    m.def("angle", &angle, "A function get angle between two 2D-vectors");
}

