CXX = g++
CXXFLAGS = -Wall -shared -std=c++14
EXTENSION = $(shell python3-config --extension-suffix)
SOFILE := _matrix$(EXTENSION)
PYBIND = $(shell python3 -m pybind11 --includes)

UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CXXFLAGS += -undefined dynamic_lookup
	MKL = -I/opt/intel/oneapi/mkl/2021.4.0/include
else
	CXXFLAGS += -fPIC
	MKL = -I/usr/include/mkl -L/usr/lib/x86_64-linux-gnu/mkl
	PYTHON = $(shell python3-config --includes)
endif

$(SOFILE):matrix.cpp
	$(CXX) $(CXXFLAGS) $(PYTHON) $(PYBIND) matrix.cpp -o $(SOFILE) $(MKL) -lblas

test: $(SOFILE)
	python -m pytest -v

clean:
	rm -f *.so performance.txt
