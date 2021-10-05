# include <iostream>
# include <vector>
# include "Line.h"

/*
class Line
{
public:
    Line();
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line() = default;
    size_t size() const;
    float x(size_t it) const;
    float & x(size_t it);
    float y(size_t it) const;
    float & y(size_t it);
private:
    // Member data.
    void check_range(size_t it) const
    { if (it >= m_size) { throw std::out_of_range("Line index out of range"); } }
    std::vector<float> m_x;
    std::vector<float> m_y;
    size_t m_size;
};
*/

Line::Line()
{
    //m_size = 0;
}

Line::Line(Line const & a)
{
    m_x = a.m_x;
    m_y = a.m_y;
    m_size = a.size();
}

Line::Line(Line       && a)
{
    std::swap(a.m_size, m_size);
    std::swap(a.m_x, m_x);
    std::swap(a.m_y, m_y);
}

Line & Line::operator=(Line const & a)
{
    m_x = a.m_x;
    m_y = a.m_y;
    m_size = a.size();
    return *this;
}

Line & Line::operator=(Line       && a)
{
    std::swap(a.m_size, m_size);
    std::swap(a.m_x, m_x);
    std::swap(a.m_y, m_y);
    return *this;
}

Line::Line(size_t size)
{
    m_size = size;
    m_x.resize(size);
    m_y.resize(size);
}

size_t Line::size() const
{
    return m_size;
}

float Line::x(size_t it) const
{
    check_range(it);
    return m_x.at(it);
}

float & Line::x(size_t it)
{
    check_range(it);
    return m_x.at(it);
}

float Line::y(size_t it) const
{
    check_range(it);
    return m_y.at(it);
}

float & Line::y(size_t it)
{
    check_range(it);
    return m_y.at(it);
}
