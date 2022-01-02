CXX = g++
CFLAGS = --std=c++17 -Wall -O3 -fPIC -shared
PY_INC= $(shell python3 -m pybind11 --includes) $(shell python3-config --includes) 
SLF_INC = -I./
NP_INC = -I$(shell python3 -c "import numpy; print(numpy.get_include())")
MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas
OUT = _matrix$(shell python3-config --extension-suffix)

.PHONY: clean all test target

all: target

target: matrix_pybind.cpp
	$(CXX) $(CFLAGS) $(SLF_INC) $(NP_INC) $(MKL_INC) $(MKL_LIB) $(PY_INC) $< -o $(OUT) -lblas

test: target
	../validate.py

clean:
	rm -f _matrix*.so
	rm -rf __pycache__