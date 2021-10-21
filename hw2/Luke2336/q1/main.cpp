#include<iostream>
#include<vector>

class Line
{
public:
    Line() {}
    Line(Line const &line): X(line.X), Y(line.Y) {}
    Line(Line &&line) : X(line.X), Y(line.Y) {}
    Line & operator=(Line const &line) {
        X = line.X;
        Y = line.Y;
        return *this;
    }
    Line & operator=(Line &&line) {
        X = line.X;
        Y = line.Y;
        return line;
    }
    Line(size_t size) {
        X.resize(size);
        Y.resize(size);
    }
    ~Line() {
        X.clear();
        Y.clear();
    }
    size_t size() const {
        return X.size();
    }
    float x(size_t it) const {
        return X.at(it);
    }
    float & x(size_t it) {
        return X.at(it);
    }
    float y(size_t it) const {
        return Y.at(it);
    }
    float & y(size_t it) {
        return Y.at(it);
    }
private:
    std::vector<float> X, Y;
};

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

