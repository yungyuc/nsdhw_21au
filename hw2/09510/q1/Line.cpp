#include "Line.hpp"


Line::Line(Line const & l)
{
    m_coord=l.m_coord;
}

Line::Line(Line && l)
{
    std::swap(l.m_coord,m_coord);
}


Line & Line::operator=(Line const & l)
{
    
    m_coord=l.m_coord;
    
    return *this;
}

Line & Line::operator=(Line && l)
{

    std::swap(l.m_coord,m_coord);

    return *this;
}

Line::Line(size_t size)
{
    m_coord.resize(size);
}

size_t Line::size()const
{
    return m_coord.size();
}

Line::~Line()=default;

float & Line::x(size_t v)
{
    return m_coord[v].first;
}

float const & Line::x(size_t v)const
{
    return m_coord[v].first;
}

float & Line::y(size_t v){
    return m_coord[v].second;
}

float const & Line::y(size_t v)const
{
    return m_coord[v].second;
}

