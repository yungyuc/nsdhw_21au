/*
  Copyright (C) 2021 Marvin Liu

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <vector>
#include <stdexcept>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>

const double eps = 1e-7;

/* return angle in radius */
double angle(const std::vector<double> &v1, const std::vector<double> &v2) {
  if (v1.size() != 2 || v2.size() != 2 ) {
    throw std::invalid_argument("The two vectors should be 2 dimension.");
  }

  if ((v1[0] == 0.0 && v1[1] == 0.0) || (v2[0] == 0.0 && v2[1] == 0.0)) {
    throw std::invalid_argument("The input should not be zero vector.");
  }

  double l1 = hypot(v1[0], v1[1]);
  double l2 = hypot(v2[0], v2[1]);

  if (l1 == HUGE_VAL || l2 == HUGE_VAL) {
    throw std::invalid_argument("The vector is too long.");
  }

  if (abs(l1) < eps || abs(l2) < eps) {
    throw std::invalid_argument("The input should not be zero vector.");
  }

  double dot = v1[0] * v2[0] + v1[1] * v2[1];
  if (isinf(dot)) {
    throw std::invalid_argument("The vector is too long.");
  }

  double vvcos = dot/l1/l2;

  double ret;
  if (vvcos > 1.0) ret = acos(1.0);
  else if (vvcos < -1.0) ret = acos(-1.0);
  else ret = acos(vvcos);

  return ret;
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "input two list, return the angle between the two vectors in rad.";
  m.def("angle", &angle, "A function return the angle between two 2d vectors.");
}
