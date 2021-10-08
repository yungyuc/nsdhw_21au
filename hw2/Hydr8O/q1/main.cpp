#include <cstddef>
#include <vector>
#include <iostream>

class Line {
    public:
        Line(Line const &oldLine) {
            xCoordinates = oldLine.xCoordinates;
            yCoordinates = oldLine.yCoordinates;
        };

        Line(Line &&oldLine) {
            xCoordinates = std::move(oldLine.xCoordinates);
            yCoordinates = std::move(oldLine.yCoordinates);
        }

        Line &Line::operator=(Line const &oldLine) {
            if (this == &oldLine) { return *this; }
            xCoordinates = oldLine.xCoordinates;
            yCoordinates = oldLine.yCoordinates;

        }

        Line &Line::operator=(Line &&oldLine) {
            if (this == &oldLine) { return *this; }
            xCoordinates = std::move(oldLine.xCoordinates);
            yCoordinates = std::move(oldLine.yCoordinates);
        }

        Line(std::size_t size) {
            numberOfPoints = size;
            xCoordinates.resize(size);
            yCoordinates.resize(size);
        };

        size_t getSize() {
            return numberOfPoints;
        }

        float getXAt(int index) {
            return xCoordinates.at(index);
        }

        float getYAt(int index) {
            return yCoordinates.at(index);
        }

        void setXAt(int index, float value) {
            xCoordinates.at(index) = value;
        }

        void setYAt(int index, float value) {
            yCoordinates.at(index) = value;
        }


    private:
        std::size_t numberOfPoints;
        std::vector<float> xCoordinates;
        std::vector<float> yCoordinates;

};



int main(int, char **) {
    std::cout << "Start" << std::endl; 
    Line line(3);
    std::cout << "Line has been initialized" << std::endl; 
    line.setXAt(0, 0); line.setYAt(0, 1);
    line.setXAt(1, 1); line.setYAt(1, 3);
    line.setXAt(2, 2); line.setYAt(2, 5);

    Line line2(line);
    line2.setXAt(0, 9);

    std::cout << "line: number of points = " << line.getSize() << std::endl;
    for (size_t it = 0; it < line.getSize(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.getXAt(it)
                  << " y = " << line.getYAt(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.getSize() << std::endl;
    for (size_t it=0; it < line.getSize(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.getXAt(it)
                  << " y = " << line2.getYAt(it) << std::endl;
    }

    return 0;
}