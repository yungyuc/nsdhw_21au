CXX = g++
INCLUDE_DIR = /opt/intel/oneapi/mkl/2021.4.0/include
INCLUDE = `python3-config --includes`

MKL_DIR = /opt/intel/oneapi/mkl/2021.4.0/lib
# MKLFLAGS = ${MKL_DIR}/libmkl_def.so \
# 	${MKL_DIR}/libmkl_avx2.so \
# 	${MKL_DIR}/libmkl_core.so \
# 	${MKL_DIR}/libmkl_intel_lp64.so \
# 	${MKL_DIR}/libmkl_sequential.so
MKLFLAGS = -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl

CXXFLAGS = -Wall -O3 -shared -std=c++11 -undefined dynamic_lookup  -I${INCLUDE_DIR} ${MKLFLAGS} ${INCLUDE}
FPIC = `python3 -m pybind11 --includes`
SOFILE = _matrix${shell python3-config --extension-suffix}

.PHONY: all
all: ${SOFILE}

${SOFILE}: matrix.cpp
	${CXX} ${FPIC} $< -o $@ ${CXXFLAGS} 

test: ${SOFILE}
	python -m pytest

clean:
	rm -rf *.so .pytest_cache __pycache__  performance.txt