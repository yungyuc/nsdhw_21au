#include <iostream>
#include <vector>

struct COORD {
  float x;
  float y;
};

class Line {
  public:
    Line();
    Line(Line const & other) : m_size(other.m_size), m_coord(other.m_coord) {}
    Line & operator=(Line const & other);
    Line(size_t size) : m_size(size), m_coord(size) {}
    ~Line() = default;
    size_t size() const { return m_size; }
    float & x(size_t it) const;
    float & x(size_t it);
    float & y(size_t it) const;
    float & y(size_t it);
  private:
    void check_range(size_t it) const;
    size_t m_size = 0;
    std::vector<COORD> m_coord;
}; /* end class Line */

Line & Line::operator=(Line const & other) {
  if (this == &other) {
    return *this;
  }

  m_size = other.m_size;
  m_coord = other.m_coord;

  return *this;
}

float & Line::x(size_t it) const {
  check_range(it);
  return const_cast<float &>(m_coord[it].x);
}

float & Line::x(size_t it) {
  check_range(it);
  return m_coord[it].x;
}

float & Line::y(size_t it) const {
  check_range(it);
  return const_cast<float &>(m_coord[it].y);
}

float & Line::y(size_t it) {
  check_range(it);
  return m_coord[it].y;
}

void Line::check_range(size_t it) const {
  if (it >= m_size) {
    throw std::out_of_range("Line index out of range");
  }
}

int main(int argc, char **argv)
{
  Line line(3);
  line.x(0) = 0; line.y(0) = 1;
  line.x(1) = 1; line.y(1) = 3;
  line.x(2) = 2; line.y(2) = 5;

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
