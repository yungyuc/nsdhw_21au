#include <pybind11/pybind11.h>
#include <cmath>

namespace py = pybind11;

double get_angle(double x1, double y1, double x2, double y2){

    double length1 = sqrt(x1*x1 + y1*y1);
    double length2 = sqrt(x2*x2 + y2*y2);

    if(length1 == 0 | length2 == 0){
        return true;
    }

    double inner = x1*x2 + y1*y2;
    double cosine = inner / (length1*length2);


    return acos(cosine);
}

PYBIND11_MODULE(_vector, m){
    m.def("The angle in radians is: ", &get_angle);
}