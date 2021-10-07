#include <pybind11/pybind11.h>
#include <cmath>

float angle_between(float x1, float y1, float x2, float y2)
{
    float dot_product, norm1, norm2, cos_val, theta;
    dot_product = x1*x2 + y1*y2;
    norm1 = std::sqrt(x1*x1 + y1*y1);
    norm2 = std::sqrt(x2*x2 + y2*y2);

    // If one of the vectors is of length 0, the input is invalid
    if(norm1*norm2 == 0)
        return -1;

    cos_val = dot_product / (norm1*norm2);
    
    // safe acos: to get rid of some problems caused by floating point precision
    if(cos_val <= -1.0)
        theta = std::acos(-1);
    else if(cos_val >= 1.0)
        theta = std::acos(1);
    else
        theta = std::acos(cos_val);

    return theta;
}

PYBIND11_MODULE(_vector, m)
{
    m.def("angle_between", &angle_between);
}
