#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <array>
#include <cmath>

namespace py = pybind11;

typedef std::array<float, 2> Vector;

float angle(const Vector &v1, const Vector &v2) {
  float v1_length = v1[0] * v1[0] + v1[1] * v1[1];
  float v2_length = v2[0] * v2[0] + v2[1] * v2[1];

  if (v1_length == 0 || v2_length == 0) {
    return -1;
  }

  float dot_product = v1[0] * v2[0] + v1[1] * v2[1];
  float cosine = dot_product / sqrt(v1_length + v2_length);

  if (cosine > 1) {
    cosine = 1;
  } else if (cosine < -1){
    cosine = -1;
  }

  return acos(cosine);
}

PYBIND11_MODULE(_vector, m) {
  m.def("angle", &angle, "A function which calculates the angle (in radians) between two vectors.");
}
