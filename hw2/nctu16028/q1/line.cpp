#include "line.hpp"

// copy constructor
Line::Line(Line const &cp)
{
    px = cp.px;
    py = cp.py;
}

// move constructor
Line::Line(Line &&mv)
{
    px = mv.px;
    mv.px.clear();
    py = mv.py;
    mv.py.clear();
}

// copy assignment operator
Line & Line::operator=(Line const &cp_rval)
{
    if(this == &cp_rval)
        return *this;
    px = cp_rval.px;
    py = cp_rval.py;
    return *this;
}

// move assignment operator
Line & Line::operator=(Line &&mv_rval)
{
    if(this == &mv_rval)
        return *this;
    px = mv_rval.px;
    mv_rval.px.clear();
    py = mv_rval.py;
    mv_rval.py.clear();
    return *this;
}

// custom constructor
Line::Line(size_t size)
{
    px.resize(size);
    py.resize(size);
}

// destructor
Line::~Line()
{
    px.clear();
    py.clear();
}

// access the number of points on the line
size_t Line::size() const
{
    return px.size();
}

// x getter
float Line::x(size_t it) const
{
    return px[it];
}

// x setter
float & Line::x(size_t it)
{
    return px[it];
}

// y getter
float Line::y(size_t it) const
{
    return py[it];
}

// y setter
float & Line::y(size_t it)
{
    return py[it];
}
