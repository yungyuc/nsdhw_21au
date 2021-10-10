CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -O3

TARGET = main
objs = *.o
txt = result.txt

all: $(exe)

Line.o: Line.cpp Line.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): main.cpp Line.o Line.hpp
	$(CXX) $(CXXFLAGS) main.cpp Line.o -o $@

.PHONY: clean run check

run: $(TARGET)
	./$(TARGET)

check: $(TARGET)
	./$(TARGET) > $(txt)

clean:
	rm -f $(objs) $(exe)