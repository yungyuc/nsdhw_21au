#include <cmath>
#include <iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;

float _vector(float x1, float y1, float x2, float y2) {
    float firstDotProduct = sqrt(x1 * x1 + y1 * y1);
    float secondDotProduct = sqrt(x2 * x2 + y2 * y2);
    float vectorProduct = x1 * x2 + y1 * y2;
    return std::acos(vectorProduct / (firstDotProduct * secondDotProduct));
}

PYBIND11_MODULE(_vector, m) {
    m.def("_vector", &_vector, "A function which computes an angle between two vectors");
}