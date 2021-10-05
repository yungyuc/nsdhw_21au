CXX = g++
CFLAGS = --std=c++11 -Wall -O3

TARGET = _vector.*.so
all: $(TARGET)

.PHONY: clean test

$(TARGET): _vector_pybind.cpp _vector.o _vector.hpp
	$(CXX) $(CFLAGS) -shared -fPIC `python3 -m pybind11 --includes` _vector_pybind.cpp _vector.o -o _vector`python3-config --extension-suffix` `python3-config --includes`

vecAng.o: _vector.cpp _vector.hpp
	$(CXX) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	python -m pytest _vector_test.py

clean: 
	rm -f *.o $(TARGET)
	rm -r __pycache__
