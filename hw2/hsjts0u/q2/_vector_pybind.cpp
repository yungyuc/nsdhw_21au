#include "_vector.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(_vector, m) {
    m.doc() = "vecAng lib: calculate angle (radians) between vectors: import _vector";

    py::class_<vec>(m, "vec")
        //.def(py::init<>())
        .def(py::init<double, double>())
        .def_readwrite("m_x", &vec::m_x)
        .def_readwrite("m_y", &vec::m_y);

    m.def("calc", &calc, "calculate angle (radians) between vectors");
}