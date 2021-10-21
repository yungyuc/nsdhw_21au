#ifndef _LINE_HPP
#define _LINE_HPP

#include <stddef.h>
#include <vector>

typedef struct _Point{
    float x;
    float y;
}Point;

class Line{
    public:
        Line() = default;
        Line(Line const &);
        Line &operator=(Line const &);
        Line(size_t size);
        ~Line() = default;

        size_t size() const;
        const float &x(size_t it) const;
        float &x(size_t it);
        const float &y(size_t it) const;
        float &y(size_t it);

    private:
    std::vector<Point> m_points;

};


#endif
