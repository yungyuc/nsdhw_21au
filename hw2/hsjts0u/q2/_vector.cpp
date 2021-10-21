#include "_vector.hpp"
#include <cmath>

double calc(vec &v1, vec &v2){
    
    double n = v1.m_x * v2.m_x + v1.m_y * v2.m_y;
    double v1_l = sqrt(v1.m_x * v1.m_x + v1.m_y * v1.m_y);
    double v2_l = sqrt(v2.m_x * v2.m_x + v2.m_y * v2.m_y);

    if(v1_l * v2_l == 0)
        return -1.0;

    double t = n / (v1_l * v2_l);
    
    if(t > 1.0)
        return 0.0;
    else if(t < -1.0)
        return 3.14;
    else
        return acos(t);
}