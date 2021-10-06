#include <pybind11/pybind11.h>
#include <vector>

namespace py = pybind11;

inline double dot(int x1, int y1, int x2, int y2) {
	return x1 * x2 + y1 * y2;
}

inline double len(int x, int y) {
	return sqrt(x * x + y * y);
}

double calAngle(int x1, int y1, int x2, int y2) {
  if ((x1 == 0 && y1 == 0) || (x2 == 0 && y2 == 0)) {
    return 0;
  }
	return acos(dot(x1, y1, x2, y2) / len(x1, y1) / len(x2, y2));
}

PYBIND11_MODULE(_angle, m) {
  m.doc() = "pybind11 example plugin";
  m.def("calAngle",
        &calAngle,
        "A function calculating the angle between two vectors in radians"
       );
}
