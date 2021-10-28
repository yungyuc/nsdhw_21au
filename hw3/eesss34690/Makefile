CXX = g++

INCLUDE_DIRS = /usr/include/mkl
MKL_DIR = /usr/lib/x86_64-linux-gnu

MKLFLAGS = ${MKL_DIR}/libmkl_def.so \
	${MKL_DIR}/libmkl_avx2.so \
	${MKL_DIR}/libmkl_core.so \
	${MKL_DIR}/libmkl_intel_lp64.so \
	${MKL_DIR}/libmkl_sequential.so

CXXFLAGS = -O3 -Wl,--no-as-needed -shared -std=c++11 -fPIC -I${INCLUDE_DIRS} ${MKLFLAGS} -ldl -lpthread -lm

MODULE = _matrix
SOFILE = ${MODULE}${shell python3-config --extension-suffix}

.PHONY: all
all: ${SOFILE}

${SOFILE}: matrix.cpp
	${CXX} `python3 -m pybind11 --includes` $< -o $@ ${CXXFLAGS}

test: ${SOFILE}
	pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache performance.txt
