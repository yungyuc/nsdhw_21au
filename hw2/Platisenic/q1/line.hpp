#pragma once
#include <cstdint>
#include <vector>
#include <algorithm>
#include <utility>

class Line
{
public:
    Line() = default; // default constructor
    Line(Line const & ); // copy constructor
    Line(Line       &&); // move constructor
    Line & operator=(Line const & ); // copy assignment operator
    Line & operator=(Line       &&); // move assignment operator
    Line(size_t size); // custom constructor
    ~Line() = default; // destructor
    size_t size() const; // get size of the line
    float   x(size_t it) const; // get x
    float & x(size_t it); // set x
    float   y(size_t it) const; // get y
    float & y(size_t it); // set y
private:
    std::vector<float> m_coord_x;
    std::vector<float> m_coord_y;
};