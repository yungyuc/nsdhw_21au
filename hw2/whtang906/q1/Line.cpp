#include <iostream>
#include "Line.h"

Line::Line(Line const & oldLine){
    this->coords = oldLine.coords;
}

Line::Line(Line && oldLine){
    this->coords = oldLine.coords;
}

Line & Line::operator=(Line const & oldLine){
    this->coords = oldLine.coords;
    return *this;
}

Line & Line::operator=(Line && src){
    this->coords = std::move(src.coords);
    return *this;
}

Line::Line(size_t size){
    this->coords.resize(size);
}

Line::~Line(){
    this->coords.clear();
}

size_t Line::size() const{
    return coords.size();
}

float & Line::x(size_t it){
    return coords.at(it).first;
}

float & Line::y(size_t it){
    return coords.at(it).second;
}

