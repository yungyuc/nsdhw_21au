#include <utility>

struct vec {
    vec(double x, double y) : m_x(x), m_y(y) {}

    double m_x;
    double m_y;
};

double calc(vec &v1, vec &v2);
