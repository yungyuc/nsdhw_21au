//source code from https://yyc.solvcon.net/en/latest/nsd/03cpp/cpp.html

# include <pybind11/pybind11.h>
# define _USE_MATH_DEFINES
# include <math.h>
# include <cmath>

namespace py = pybind11;
float get_Angle (float m_x1, float m_x2, float m_y1, float m_y2)
{
    if ((!m_x1 && !m_y1) || (!m_x2 && !m_y2))
    {
	throw std::invalid_argument("zero length for one of the vector");
    }
    // calculate a^2, b^2, c^2
    float a_sq = m_x1* m_x1 + m_y1* m_y1, b_sq = m_x2* m_x2 + m_y2* m_y2;
    float c_sq = (m_x1 - m_x2)* (m_x1 - m_x2) + (m_y1 - m_y2)* (m_y1 - m_y2);
    // c^2 = a^2 + b^2 - 2*a*b*cos\theta
    float _cos = (a_sq + b_sq - c_sq)/ (2* sqrt(a_sq)* sqrt(b_sq));
    return acos(_cos)* 180/ M_PI; 
}

PYBIND11_MODULE(_vector, m){
	m.doc() = "pybind11 angle getter";
	m.def("get_Angle", &get_Angle, "calculate angle for 2 vector");
}
