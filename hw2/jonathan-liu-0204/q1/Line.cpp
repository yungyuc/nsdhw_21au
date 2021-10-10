#include "Line.hpp"
#include <iostream>

Line::Line(){}

Line::Line(Line const &copy){
    this -> X.assign(copy.X.begin(), copy.X.end());
    this -> Y.assign(copy.Y.begin(), copy.Y.end());
}

Line::Line(Line const &move){
    this -> X.assign(move.X.begin(), move.X.end());
    this -> Y.assign(move.Y.begin(), move.Y.end());
}

Line & Line::operator=(Line const &copy_a){
    if (this != &copy_a){
        X = copy_a.X;
        Y = copy_a.Y;
    }
    return *this;
}

Line & Line::operator=(Line &&move_a){
    if (this != &move_a){
        X = move_a.X;
        Y = move_a.Y;
    }
    return *this;
}

Line::Line(size_t size){
    X.resize(size);
    Y.resize(size);
}

Line::~Line(){
    X.clear();
    Y.clear();
}

size_t Line::size() const{
    return X.size();
}

const float & Line::x(size_t it) const{
    return X[it];
}

float & Line::x(size_t it) {
    return X[it];
}

const float & Line::y(size_t it) const {
    return Y[it];
}

float & Line::y(size_t it) {
    return Y[it];
}