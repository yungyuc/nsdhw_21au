#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>
#include <cmath>

float cal_angle(const std::vector<float> &x, const std::vector<float> &y){
  float x_length = sqrt(x.at(0) * x.at(0) + x.at(1) * x.at(1));
  float y_length = sqrt(y.at(0) * y.at(0) + y.at(1) * y.at(1));
  if(x_length == 0 || y_length == 0){
    return -666;
  }

  float xdoty = x.at(0) * y.at(0) + x.at(1) * y.at(1);
  float costheta = xdoty / (x_length * y_length);

  float angle_ra = acos(costheta);
  std::cout<<angle_ra<<std::endl;

  return angle_ra;
}

PYBIND11_MODULE(_vector, m){
  m.doc() = "to calculate angle";
  m.def("cal_angle", &cal_angle, "calculate angle between two vectors");
}


