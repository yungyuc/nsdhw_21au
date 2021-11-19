CXX = g++

INCLUDE = `python3-config --includes`

# MKL_DIR = /opt/intel/oneapi/mkl/2021.4.0/lib
MKL_DIR = /usr/lib/x86_64-linux-gnu/mkl
# MKL_INCLUDE = /opt/intel/oneapi/mkl/2021.4.0/include
MKL_INCLUDE = /usr/include/mkl
# MKLFLAGS = -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl -lblas
MKLFLAGS = -L /usr/lib/x86_64-linux-gnu/mkl -lblas

CXXFLAGS = -Wall -O3 -shared -std=c++17 -fPIC -I${MKL_INCLUDE} ${MKLFLAGS} ${INCLUDE}
FPIC = `python3 -m pybind11 --includes`
SOFILE = _matrix${shell python3-config --extension-suffix}

.PHONY: all
all: ${SOFILE}

${SOFILE}: matrix.cpp CustomAllocator.h
	${CXX} ${FPIC} $< -o $@ ${CXXFLAGS} 

test: ${SOFILE}
	python -m pytest ../validate.py

clean:
	rm -rf *.so .pytest_cache __pycache__