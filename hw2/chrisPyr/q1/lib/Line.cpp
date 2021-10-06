#include "Line.hpp"

Line::Line(size_t size):m_points(size){
}

Line::Line(Line const &ref){
    m_points = ref.m_points;
}

Line &Line::operator=(const Line &ref){
    if(this == &ref){
        return *this;
    }
    m_points = ref.m_points;
    return *this;
}

size_t Line::size() const{
    return m_points.size();
}

const float &Line::x(size_t it) const{
    return m_points.at(it).x;
}
float &Line::x(size_t it) {
    return m_points.at(it).x;
}

const float &Line::y(size_t it) const{
    return m_points.at(it).y;
}
float &Line::y(size_t it) {
    return m_points.at(it).y;
}
