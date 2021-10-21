#include "Line.hpp"

Line::Line(){}

// copy
Line::Line(Line const &copied){
    this->m_x.assign(copied.m_x.begin(), copied.m_x.end());
    this->m_y.assign(copied.m_y.begin(), copied.m_y.end());
}

// move
Line::Line(Line &&moved){
    this->m_x.assign(moved.m_x.begin(), moved.m_x.end());
    this->m_y.assign(moved.m_y.begin(), moved.m_y.end());
    moved.m_x.clear();
    moved.m_y.clear();
}

// copy assignment
Line & Line::operator=(Line const &cp_assign){
    this->m_x = cp_assign.m_x;
    this->m_y = cp_assign.m_y;
    return *this;
}

// move assignment
Line & Line::operator=(Line &&mv_assign){
    this->m_x = mv_assign.m_x;
    this->m_y = mv_assign.m_y;
    mv_assign.m_x.clear();
    mv_assign.m_y.clear();
    return *this;
}

Line::Line(size_t size){
    this->m_x.resize(size);
    this->m_y.resize(size);
}

Line::~Line(){
    this->m_x.clear();
    this->m_y.clear();
}

size_t Line::size() const {
    return this->m_x.size();
}

const float & Line::x(size_t it) const {
    return this->m_x[it];
}
float & Line::x(size_t it){
    return this->m_x[it];
}

const float & Line::y(size_t it) const {
    return this->m_y[it];
}

float & Line::y(size_t it){
    return this->m_y[it];
}