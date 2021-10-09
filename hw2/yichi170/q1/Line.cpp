#include "Line.hpp"


Line::Line(){}

 // copy constructor
Line::Line(Line const & other) : X(other.X), Y(other.Y) {}

// move constructor
Line::Line(Line && other) : X(std::move(other.X)), Y(std::move(other.Y)) {}
    
Line & Line::operator=(Line const & other){
    if (this != &other) X = other.X, Y = other.Y;
    return *this;
}
Line & Line::operator=(Line && other) {
    if (this != &other) X = std::move(other.X), Y = std::move(other.Y);
    return *this;
}
Line::Line(size_t size) {
    X.resize(size), Y.resize(size);
}

Line::~Line() {
    X.clear(), Y.clear();
}

size_t Line::size() const {
    return X.size();
}

const float & Line::x(size_t it) const {
    return X.at(it);
} // getter

float & Line::x(size_t it) {
    return X.at(it);
} // setter

const float & Line::y(size_t it) const {
    return Y.at(it);
} // getter

float & Line::y(size_t it) {
    return Y.at(it);
} // setter