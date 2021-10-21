#include <vector>

class Line
{
public:
    Line();
    Line(Line const &line) : x_vec(line.x_vec), y_vec(line.y_vec) {};
    Line(Line &&);
    Line(size_t size) : x_vec(size), y_vec(size) {};
    ~Line() {};
    Line& operator=(Line const&);
    Line& operator=(Line &&);
    size_t size() const { return x_vec.size(); };
    const float& x(size_t it) const { return x_vec[it]; };
    const float& y(size_t it) const { return y_vec[it]; };
    float& x(size_t it) { return x_vec[it]; };
    float& y(size_t it) { return y_vec[it]; };
private:
    std::vector<float> x_vec;
    std::vector<float> y_vec;
}; /* end class Line */