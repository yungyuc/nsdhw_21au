#include <iostream>

class Line
{
public:
    Line();
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t );
    ~Line();
    size_t size() const;
    // float & x(size_t it) const;
    float & x(size_t it);
    // float & y(size_t it) const;
    float & y(size_t it);
private:
    std::vector<float> m_data;
    // Member data.
}; /* end class Line */