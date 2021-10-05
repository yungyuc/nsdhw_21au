#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cmath>

typedef std::array<float, 2> Vector;

float angle(const Vector &v1, const Vector &v2) {
  if((v1[0] == 0.0 && v1[1] == 0.0) || (v2[0] == 0.0 && v2[1] == 0.0))
    throw std::invalid_argument("zero-length 2-vector");

  float dot = v1[0] * v2[0] + v1[1] * v2[1];
  float det = v1[0] * v2[1] - v1[1] * v2[0];
  return atan2(det, dot);
}

PYBIND11_MODULE(_vector, m) {
  m.def("angle", &angle, "calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system");
}
