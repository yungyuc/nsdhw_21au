#include <iostream>
#include <vector>

class Line
{
public:
    Line() = default;
    Line(size_t size) { m_points.resize(size); }
    ~Line() = default;

    Line(Line const &);
    Line(Line &&);
    Line &operator=(Line const &);
    Line &operator=(Line &&);

    size_t size() const { return m_points.size(); }

    float const &x(size_t it) const { return m_points[it].first; }
    float &x(size_t it) { return m_points[it].first; }

    float const &y(size_t it) const { return m_points[it].second; }
    float &y(size_t it) { return m_points[it].second; }

private:
    std::vector<std::pair<float, float>> m_points;
}; /* end class Line */

Line::Line(Line const &other) : m_points(other.m_points) {}
Line::Line(Line &&other) : m_points(std::move(other.m_points)) {}

Line &Line::operator=(Line const &other) 
{ 
    m_points = other.m_points; 
    return *this; 
}

Line &Line::operator=(Line &&other) 
{ 
    m_points = std::move(other.m_points);
    return *this; 
}

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0;
    line.y(0) = 1;
    line.x(1) = 1;
    line.y(1) = 3;
    line.x(2) = 2;
    line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it = 0; it < line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it = 0; it < line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}