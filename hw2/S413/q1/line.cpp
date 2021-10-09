#include<iostream>
#include<ostream>
#include<vector>

class Line
{
public:
    Line(int len) { vect.resize(len, std::vector<float>(2)); }
    Line(Line const & )=default;
    Line(Line       &&)=default;
    Line & operator=(Line const & )=default;
    Line & operator=(Line       &&)=default;
    Line(size_t size);
    ~Line() { std::vector<std::vector<float>>().swap(vect); }
    size_t size() const { return vect.size(); }
    float  x(size_t it) const { return vect[it][0]; }
    float & x(size_t it) { return vect[it][0]; }
    float  y(size_t it) const { return vect[it][1]; }
    float & y(size_t it) {return vect[it][1]; }
private:
    // Member data.
    std::vector<std::vector<float>> vect;
}; /* end class Line */

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}

