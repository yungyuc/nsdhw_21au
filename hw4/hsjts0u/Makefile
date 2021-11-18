CXX = g++
CFLAGS = --std=c++11 -Wall -O3
INCLUDES = $(shell python3 -m pybind11 --includes) $(shell python3-config --includes) 
MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas
OUT = _matrix$(shell python3-config --extension-suffix)
TARGET = main

.PHONY: clean test

$(TARGET): _matrix_pybind.cpp _matrix.cpp _matrix.hpp
        $(CXX) $(CFLAGS) -shared -fPIC $(INCLUDES) $(MKL_INC) $(MKL_LIB) $^ -o $(OUT) -lblas

clean:
        rm -f _matrix*.so performance.txt
        rm -rf __pycache__ .pytest_cache

test: $(TARGET)
        python3 -m pytest ../validate.py