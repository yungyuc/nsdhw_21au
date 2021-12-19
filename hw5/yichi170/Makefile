CXX = g++
CXXFLAGS = -std=c++17 -g -O3 -Wall -fPIC -shared
PYBIND_INC = `python3 -m pybind11 --includes` `python3-config --includes`

NUMPY_INC := -I$(shell python3 -c "import numpy; print(numpy.get_include())")
MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas

target: ../mod.cpp
	$(CXX) $(CXXFLAGS) -I./ $(NUMPY_INC) $(PYBIND_INC) $(MKL_INC) $(MKL_LIB) $< -o _matrix`python3-config --extension-suffix` -lblas

.PHONY: all clean test

all: target

clean:
	rm -f *.so
	rm -rf ../__pycache__ ../.pytest_cache

test: target
	python3 -m pytest ../validate.py