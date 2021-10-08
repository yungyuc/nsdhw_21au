#include<iostream>
#include<vector>
using namespace std;

class Line
{
public:
    Line(){cout<<"ctor!!"<<endl;}
    Line(size_t size):a(size), b(size){}
    Line(const Line& rhs){a = rhs.a, b = rhs.b, size_ = rhs.size_;}
    ~Line(){}
    size_t size() const {return size_;};
    float& x(size_t it){return a.at(it);};
    float& y(size_t it){return b.at(it);};

private:
    vector<float>a, b;
    int size_ = a.size();
};

int main(int, char**)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it = 0; it < line.size(); ++it)
    {
        std::cout << "point " << it << ":"
            << " x = " << line.x(it)
            << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it = 0; it < line.size(); ++it)
    {
        std::cout << "point " << it << ":"
            << " x = " << line2.x(it)
            << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}