#include <cstddef>
#include <vector>

class Line {

public:
    Line();
    Line(Line const &);
    Line(Line && other);
    Line & operator=(Line const & other);
    Line & operator=(Line && other);
    Line(size_t size);
    ~Line();
    size_t size() const;
    const float & x(size_t it) const;
    float & x(size_t it);
    const float & y(size_t it) const;
    float & y(size_t it);

private:
    // Member data.
    std::vector<float> X, Y;

}; /* end class Line */