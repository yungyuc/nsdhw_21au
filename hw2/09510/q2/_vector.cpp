#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cmath>

using namespace std;


double Angle(const vector<double> v1,const vector<double> &v2)
{
    //NOT 2D
    if((v1.size()!=2)|| v2.size()!=2)
    {
        return -1;
    }


    double v1_len = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
    double v2_len = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    double product = v1[0] * v2[0] + v1[1] * v2[1];

    //avoid zero length problem
    if((v1_len==0) || (v2_len==0))
    {
        return -1;
    }
    double cos = product/(v1_len * v2_len);

    //avoid little error
    if(cos>1){cos=1;}
    else if(cos<-1){cos=-1;}


    double ang=acos(cos);

    return ang;

}


PYBIND11_MODULE(_vector, m) {
    m.doc() = "calculate angle";
    m.def("Angle", &Angle, "calculates angle");
} 