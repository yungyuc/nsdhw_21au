CXX = g++
# INCLUDE_DIR = /usr/include/mkl
INCLUDE_DIR = /opt/intel/oneapi/mkl/2021.4.0/include

INCLUDE = `python3-config --includes`

# MKL_DIR = /usr/lib/x86_64-linux-gnu
MKL_DIR = /opt/intel/oneapi/mkl/2021.4.0/lib
# MKLFLAGS = ${MKL_DIR}/libmkl_def.so \
# 	${MKL_DIR}/libmkl_avx2.so \
# 	${MKL_DIR}/libmkl_core.so \
# 	${MKL_DIR}/libmkl_intel_lp64.so \
# 	${MKL_DIR}/libmkl_sequential.so

CXXFLAGS = -Wall -O3 -shared -std=c++11 -fPIC -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas ${INCLUDE}
FPIC = `python3 -m pybind11 --includes`
SOFILE = _matrix${shell python3-config --extension-suffix}

.PHONY: all
all: ${SOFILE}

${SOFILE}: matrix.cpp
	${CXX} ${FPIC} $< -o $@ ${CXXFLAGS} 

test: ${SOFILE}
	python -m pytest test_matrix.py -v

clean:
	rm -rf *.so .pytest_cache __pycache__  performance.txt