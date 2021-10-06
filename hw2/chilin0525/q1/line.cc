#include"line.h"
#include<iostream>
Line::Line(size_t size){
    this->point.resize(size);
}

Line::Line(Line const & src){
    this->point=src.point;
}

// move from src to this
Line::Line(Line && src){
    this->point = std::move(src.point);
}

Line & Line::operator=(Line const & src){
    this->point = src.point;
    return *this;
}

// move from src to this
Line & Line::operator=(Line && src){
    this->point = std::move(src.point);
    return *this;
}

Line::~Line(){
    this->point.clear();
}

size_t Line::size() const{
    return point.size();
}

// add const to return value
// since the member function is declared const
const float & Line::x(size_t it) const{
    return point.at(it).first;
}

float & Line::x(size_t it){
    return point.at(it).first;
}

const float & Line::y(size_t it) const{
    return point.at(it).second;
}

float & Line::y(size_t it){
    return point.at(it).second;
}
