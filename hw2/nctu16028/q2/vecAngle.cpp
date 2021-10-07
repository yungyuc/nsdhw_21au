#include <pybind11/pybind11.h>
#include <cmath>

float angle_between(float x1, float y1, float x2, float y2)
{
    float dot_product, norm1, norm2, cos_val, theta;
    dot_product = x1*x2 + y1*y2;
    norm1 = std::sqrt(x1*x1 + y1*y1);
    norm2 = std::sqrt(x2*x2 + y2*y2);
    cos_val = dot_product / (norm1*norm2);
    theta = std::acos(cos_val);

    return theta;
}

PYBIND11_MODULE(vecAngle, m)
{
    m.def("angle_between", &angle_between);
}
