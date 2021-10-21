#include <iostream>
#include <cmath>
#include <math.h>
#include <pybind11/pybind11.h>

float calculate_angle(std::pair<float, float> v1, std::pair<float, float> v2) {
  float v1_len = sqrt(v1.first*v1.first + v1.second*v1.second);
  float v2_len = sqrt(v2.first*v2.first + v2.second*v2.second);
  if ((v1_len == 0.0) || (v2_len == 0.0)) {
    throw std::invalid_argument("zero-length 2-vector (invalid input)");
  }
  float dot_product = v1.first * v2.first + v1.second * v2.second;

  return acos(dot_product/(v1_len * v2_len));
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system"; 
    m.def("calculate_angle", &calculate_angle, "function that calculate the angle");
}