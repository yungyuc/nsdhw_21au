#include "line.hpp"
Line::Line(Line const & other){
    m_coord = other.m_coord;
}

Line::Line(Line && other){
    std::swap(other.m_coord,m_coord);
}

Line & Line::operator=(Line const & other){
    m_coord = other.m_coord;
    return *this;
}

Line & Line::operator=(Line && other){
    std::swap(other.m_coord,m_coord);
    return *this;
}

Line::Line(size_t size){
    m_coord.resize(size);
}
    
Line::~Line(){
    m_coord.clear();
}

size_t Line::size()const{
    return m_coord.size();
}

float & Line::x(size_t it){
    check_range(it);
    return m_coord[it].first;
}

float const & Line::x(size_t it)const{
    check_range(it);
    return m_coord[it].first;
}

float & Line::y(size_t it){
    check_range(it);
    return m_coord[it].second;
}

float const & Line::y(size_t it)const{
    check_range(it);
    return m_coord[it].second;
}

