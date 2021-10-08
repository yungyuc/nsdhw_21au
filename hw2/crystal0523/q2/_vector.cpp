#include<math.h>
#include<iostream>
#include <pybind11/pybind11.h>
using namespace std;
namespace py = pybind11;

float CalRadian(float x1, float y1, float x2, float y2)
{
    float vec = (y2-y1)/(x2-x1);
    float radian = atan(vec);
    return radian;
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "pybind11 plugin";
    m.def("_vector", &CalRadian, "compute radian between 2 vectors"); //function name 
}


