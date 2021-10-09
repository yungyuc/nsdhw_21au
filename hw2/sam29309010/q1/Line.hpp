#ifndef _LINE
#define _LINE

#include <vector>

struct Point{
    float x;
    float y;
};

class Line
{
public:
    Line();
    Line(Line const & );
    // Line(Line       &&);
    Line & operator=(Line const & );
    // Line & operator=(Line       &&);
    Line(size_t size);
    ~Line();
    size_t size() const;
    float & x(size_t it) const;
    float & x(size_t it);
    float & y(size_t it) const;
    float & y(size_t it);
private:
    // Member data.
    std::vector<Point>* _pdata;
}; /* end class Line */

#endif