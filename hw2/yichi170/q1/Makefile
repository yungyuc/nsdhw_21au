CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -O3

exe = main
objs = *.o
txt = result.txt

all: $(exe)

Line.o: Line.cpp Line.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(exe): main.o Line.o
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean run check

clean:
	rm -f $(objs) $(exe)

check: $(TARGET)
	./$(exe) > $(txt)

run: $(TARGET)
	./$(exe)
