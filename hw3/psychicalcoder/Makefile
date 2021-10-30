CXX = g++
CXXFLAGS = -Wall -O3 --std=c++14
INCLUDES = $(shelll python3 -m pybind11 --includes) $(shell python3-config --includes)
MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu/mkl -lblas
TARGET = _matrix$(shell python3-config --extension-suffix)

.PHONY: all test clean
all: clean $(TARGET)


$(TARGET): _matrix.cpp _matrix.h
	$(CXX) $(CXXFLAGS) -shared -fPIC $(INCLUDES) $^ -o $(TARGET) $(MKL_INC) $(MKL_LIB)

clean:
	rm -rf __pycache__ _matrix*.so .pytest_cache *.o performance.txt

test: $(TARGET)
	python3 -m pytest test_matrix.py -v
