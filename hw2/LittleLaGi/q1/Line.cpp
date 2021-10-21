#include "Line.hpp"

Line::Line(Line &&line){

}

Line& Line::operator=(Line const &line){
    //deal with assigning to itself
    std::vector<float> tmp_x(line.x_vec);
    std::vector<float> tmp_y(line.y_vec);
    x_vec.clear();
    y_vec.clear();
    x_vec.insert(x_vec.begin(), tmp_x.begin(), tmp_y.begin());
    y_vec.insert(y_vec.begin(), tmp_y.begin(), tmp_y.begin());
    return *this;
}

Line& Line::operator=(Line &&line){
    
    
    return *this;
}