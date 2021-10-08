#include<math.h>
#include<iostream>
#include <pybind11/pybind11.h>
using namespace std;
namespace py = pybind11;

float CalRadian(float x1, float y1, float x2, float y2)
{
    float dot_product = x1*x2+y1*y2, len1 = sqrt(x1*x1+y1*y1), len2 = sqrt(x2*x2+y2*y2);
    float radian = acos(dot_product/(len1*len2));
    return radian;
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "pybind11 plugin";
    m.def("_vector", &CalRadian, "compute radian between 2 vectors"); //function name 
}


