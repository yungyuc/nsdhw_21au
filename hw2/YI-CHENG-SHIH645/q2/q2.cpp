#include <iostream>
#include <cmath>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Vector2D
{
    float x, y;
public:
    Vector2D(float x, float y) : x(x), y(y) {}
    float operator*(const Vector2D & other) const {
        return x*other.x + y*other.y;
    }
    float vec_len() const {
        float power = 2;
        return sqrtf(powf(x, power) + pow(y, power));
    }
};

float safe_acos(float value) {
    if (value<=-1.0) {
        return M_PI;
    } else if (value>=1.0) {
        return 0;
    } else {
        return acos(value);
    }
}

float angle(const Vector2D & v1, const Vector2D & v2) {
    float dot = v1 * v2;
    float denominator = v1.vec_len() * v2.vec_len();
    return safe_acos(dot/denominator);
}

PYBIND11_MODULE(_vector, module) {
    module.def("angle", &angle);
    py::class_<Vector2D>(module, "Vector2D")
            .def(py::init<float, float>())
            .def("vec_len", &Vector2D::vec_len);
}
