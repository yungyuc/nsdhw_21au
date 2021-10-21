//
// Created by 施奕成 on 2021/10/10.
//

#ifndef Q1_LINE_H
#define Q1_LINE_H
#include <vector>

struct Point
{
    float x, y;
};

class Line
{
public:
    Line(size_t size);
    size_t size() const;
    float & x(size_t it);
    float & y(size_t it);
private:
    // Member data.
    std::vector<Point> m_points;
}; /* end class Line */
#endif //Q1_LINE_H
