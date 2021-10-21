#include <pybind11/pybind11.h>

#include "vector.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(_vector, m) {
    m.doc() = "Vector utilities";

    m.def("angle_between_r2", &angle_between_r2, "The angle (in radians) "
          "between two vectors in the 2-dimensional Cartesian coordinate "
          "system", "u"_a, "v"_a, py::pos_only());
}
