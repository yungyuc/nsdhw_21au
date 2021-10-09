#include <iostream>
#include <vector>

class Line
{
public:
    Line();
    Line(size_t size);
    ~Line();
    Line(Line const &rhs) : m_coord(rhs.m_coord) {}
    Line(Line &&rhs) : m_coord(std::move(rhs.m_coord)) {}
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    size_t size() const;
    float & x(size_t it);
    float & y(size_t it);
    const float & x(size_t it) const;
    const float & y(size_t it) const;
private:
    // Member data.
    std::vector<std::pair<float, float>> m_coord;
}; /* end class Line */

Line::Line() = default;
Line::~Line() = default;

size_t Line::size() const
{
    return m_coord.size();
}

Line::Line(size_t size)
{
    m_coord.resize(size); 
}

Line & Line::operator=(Line const &l)
{
    if (this == &l) { return *this; }
    m_coord = l.m_coord; 
    return *this;
}

Line & Line::operator=(Line &&l)
{
    if (this == &l) { return *this; }
    m_coord = std::move(l.m_coord);
    return *this;
}

const float &Line::x(size_t it) const
{
    return  m_coord.at(it).first;
}

float &Line::x(size_t it)
{
    return  m_coord.at(it).first;
}

const float &Line::y(size_t it) const
{
    return  m_coord.at(it).second;
}

float &Line::y(size_t it)
{
    return m_coord.at(it).second;
}

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;


    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}
