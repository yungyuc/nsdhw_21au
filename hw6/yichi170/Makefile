CXX = g++
CXXFLAGS = -std=c++17 -g -O3 -Wall -shared -fPIC
PYBIND_INC = `python3 -m pybind11 --includes` `python3-config --includes`
MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas

target: main.cpp
	$(CXX) $(CXXFLAGS) -I./ $(PYBIND_INC) $(MKL_INC) $(MKL_LIB) $< -o _matrix`python3-config --extension-suffix` -lblas

.PHONY: all clean test

all: target

clean:
	rm -f *.o *.so
	rm -rf __pycache__ .pytest_cache

test: target
	python3 -m pytest ../validate.py
