#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

double angle_two_vec(std::vector<double> v1, std::vector<double> v2){
    double dotproduct = 0.0;
    double v1norm = 0.0;
    double v2norm = 0.0;
    if(v1.size() != 2 || v2.size() != 2){
        std::cerr << "not 2-dim vector" << std::endl;
        throw std::invalid_argument("not 2-dim vector");
        return 0.0;
    }

    for(size_t i=0;i<v1.size();i++){
        dotproduct += v1[i] * v2[i];
        v1norm += v1[i] * v1[i];
        v2norm += v2[i] * v2[i];
    }
    
    v1norm = sqrt(v1norm);
    v2norm = sqrt(v2norm);

    float costheta = dotproduct / (v1norm * v2norm);
    float theta = acos(costheta);

    return theta;
}

PYBIND11_MODULE(cal_vector, m) {
    m.doc() = "calulate vector plugin";
    m.def("angle_two_vec", &angle_two_vec, "A function which calculates angle of two vectors");
}