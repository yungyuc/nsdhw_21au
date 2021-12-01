CXX := g++
CXXFLAGS := -O3 -Wall -shared -std=c++17 -fPIC
TARGET = _matrix.*.so
NUMPY_PATH := $(shell python3 -c "import numpy; print(numpy.get_include())")
INCLUDE_DIRS = /usr/include/mkl
#INCLUDE_DIRS = /opt/intel/mkl/include
MKL_LIB=/usr/lib/x86_64-linux-gnu -lblas

all: target

target: ../mod.cpp
		$(CXX) $(CXXFLAGS) -I./ -I$(NUMPY_PATH) `python3 -m pybind11 --includes` $< -o _matrix`python3-config --extension-suffix` -I${INCLUDE_DIRS} -L${MKL_LIB} -lpthread
#test: $(TARGET)
#	python3 -m pytest -v

.PHONY: clean
clean:
	rm -rf *.o *.so __pycache__

