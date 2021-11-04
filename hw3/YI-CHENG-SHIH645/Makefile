#CXX = g++
#CXXFLAGS = -Wall -shared -std=c++11 -undefined dynamic_lookup
#PYBIND = $(shell python3 -m pybind11 --includes)
#EXTENSION = $(shell python3-config --extension-suffix)
#MKL = -I/opt/intel/oneapi/mkl/2021.4.0/include
#SOFILE = matrix$(EXTENSION)

CXX = g++
CXXFLAGS = -Wall -shared -std=c++11 -fPIC
PYBIND = `python3 -m pybind11 --includes`
PYTHON = `python3-config --includes`
EXTENSION = $(shell python3-config --extension-suffix)
MKL = -I/usr/include/mkl -L/usr/lib/x86_64-linux-gnu/mkl
SOFILE = _matrix$(EXTENSION)

$(SOFILE):matrix.cpp
	$(CXX) $(CXXFLAGS) $(PYTHON) $(PYBIND) matrix.cpp -o $(SOFILE) $(MKL) -lblas

test: $(SOFILE)
	python -m pytest -v

clean:
	rm -f *.so performance.txt