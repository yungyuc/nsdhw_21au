#CXX = g++
#CXXFLAGS = -Wall -shared -std=c++11 -undefined dynamic_lookup
#PYBIND = $(shell python3 -m pybind11 --includes)
#EXTENSION = $(shell python3-config --extension-suffix)
#MKL = -I/opt/intel/oneapi/mkl/2021.4.0/include

CXX = g++
CXXFLAGS = -03 -Wall -shared -std=c++11 -fPIC
PYBIND = $(shell python3 -m pybind11 --includes)
EXTENSION = $(shell python3-config --extension-suffix)
MKL = -I/usr/include/mkl -L/usr/lib/x86_64-linux-gnu/mkl

_matrix:matrix.cpp
	$(CC) $(CXXFLAGS) $(PYBIND) matrix.cpp -o matrix$(EXTENSION) $(MKL) -lblas

test:
	python -m pytest -v

clean:
	rm -f *.so performance.txt