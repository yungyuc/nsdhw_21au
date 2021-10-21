#include "line.hpp"

Line::Line(Line const & other){
    m_coord_x.resize(other.m_coord_x.size());
    m_coord_y.resize(other.m_coord_y.size());
    std::copy(other.m_coord_x.begin(), other.m_coord_x.end(), m_coord_x.begin());
    std::copy(other.m_coord_y.begin(), other.m_coord_y.end(), m_coord_y.begin());
}

Line::Line(Line && other):
    m_coord_x(std::move(other.m_coord_x)),
    m_coord_y(std::move(other.m_coord_y))
    {}

Line & Line::operator=(Line const & other){
    if(this == &other) { return *this; }
    m_coord_x.resize(other.m_coord_x.size());
    m_coord_y.resize(other.m_coord_y.size());
    std::copy(other.m_coord_x.begin(), other.m_coord_x.end(), m_coord_x.begin());
    std::copy(other.m_coord_y.begin(), other.m_coord_y.end(), m_coord_y.begin());
    return *this;
}

Line & Line::operator=(Line && other){
    if(this == &other) { return *this; }
    m_coord_x = std::move(other.m_coord_x);
    m_coord_y = std::move(other.m_coord_y);
    return *this;
}

Line::Line(size_t size){
    m_coord_x.resize(size);
    m_coord_y.resize(size);
}

size_t Line::size() const{
    return m_coord_x.size();
}

float Line::x(size_t it) const{
    return m_coord_x.at(it);
}

float & Line::x(size_t it){
    return m_coord_x.at(it);
}

float Line::y(size_t it) const{
    return m_coord_y.at(it);
}

float & Line::y(size_t it){
    return m_coord_y.at(it);
}