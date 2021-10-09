#include <iostream>
#include <vector>

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
    float const & x(size_t it) const;//getter
    float & x(size_t it);//setter
    float const & y(size_t it) const;//getter
    float & y(size_t it);//setter
private:
    std::vector<std::pair < float,float> > m_coord;
    // Member data.
}; /* end class Line */