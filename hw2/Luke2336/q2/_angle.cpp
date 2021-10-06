#include <pybind11/pybind11.h>
#include <vector>

namespace py = pybind11;

inline double dot(const std::vector<double> &v1, const std::vector<double> &v2) {
	return v1.at(0) * v2.at(0) + v1.at(1) * v2.at(1);
}

inline double len(const std::vector<double> &v) {
	return sqrt(v.at(0) * v.at(0) + v.at(1) * v.at(1));
}

double calAngle(const std::vector<double> &v1, const std::vector<double> &v2) {
	return acos(dot(v1, v2) / len(v1) / len(v2));
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "pybind11 example plugin";
  m.def("calAngle",
        &calAngle,
        "A function calculating the angle between two vectors in radians"
       );
}