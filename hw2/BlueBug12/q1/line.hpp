#ifndef _LINE_HPP_
#define _LINE_HPP_
#include <utility>
#include <vector>
#include <stdexcept>

typedef std::pair<float,float> pff;

class Line
{
public:
    Line() = default;
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line();
    size_t size() const;
    float const & x(size_t it) const;
    float & x(size_t it);
    float const & y(size_t it) const;
    float & y(size_t it);
private:
    void check_range(size_t it)const{
        if(it >= m_coord.size() || it < 0)
            throw std::out_of_range("Line index out of range");
    }
    std::vector<pff>m_coord;
}; /* end class Line */


#endif
