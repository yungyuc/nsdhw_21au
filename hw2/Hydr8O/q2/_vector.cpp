#include <cmath>
#include <math.h>
#include <iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;

float _vector(py::list vector1, py::list vector2) {
    float x1 = vector1[0].cast<float>();
    float x2 = vector2[0].cast<float>();
    float y1 = vector1[1].cast<float>();
    float y2 = vector2[1].cast<float>();

    float firstDotProduct = sqrt(x1 * x1 + y1 * y1);
    float secondDotProduct = sqrt(x2 * x2 + y2 * y2);
    float vectorProduct = x1 * x2 + y1 * y2;
    return std::acos(vectorProduct / (firstDotProduct * secondDotProduct));
}

PYBIND11_MODULE(_vector, m) {
    m.def("_vector", &_vector, "A function which computes an angle between two vectors");
}