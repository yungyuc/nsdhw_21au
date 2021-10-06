#pragma once
#include <vector>

class Line
{
public:
    Line() = default;                 // default construtor
    Line(Line const & );              // copy constructor
    Line(Line       &&);              // move constructor
    Line & operator=(Line const & );  // copy assignment operator
    Line & operator=(Line       &&);  // move assignment operator
    Line(size_t size);                // custom constructor
    ~Line();                          // destructor
    size_t size() const;              // access the number of points on the line
    float   x(size_t it) const;       // x getter
    float & x(size_t it);             // x setter
    float   y(size_t it) const;       // y getter
    float & y(size_t it);             // y setter
private:
    // Member data.
    std::vector<float> px;
    std::vector<float> py;
}; /* end class Line */
