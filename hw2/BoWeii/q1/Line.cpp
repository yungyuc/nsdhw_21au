#include "Line.hpp"


Line::Line(Line const &item){
    _data = item._data;
}
Line &Line::operator=(Line const &item){
    _data=item._data;
    return *this;
}

Line::Line(size_t size):_data(size),_size(size){}

size_t Line::size() const{
    return _size;
}

float & Line::x(size_t index){
    return _data.at(index).x;
}

const float & Line::x(size_t index) const{
    return _data.at(index).x;
}

float & Line::y(size_t index){
    return _data.at(index).y;
}

const float & Line::y(size_t index) const{
    return _data.at(index).y;
}