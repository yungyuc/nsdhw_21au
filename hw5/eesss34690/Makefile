CXX = g++

INCLUDE_DIRS = /usr/include/mkl
MKL_DIR = /usr/lib/x86_64-linux-gnu

MKLFLAGS = ${MKL_DIR}/libmkl_def.so \
	${MKL_DIR}/libmkl_avx2.so \
	${MKL_DIR}/libmkl_core.so \
	${MKL_DIR}/libmkl_intel_lp64.so \
	${MKL_DIR}/libmkl_sequential.so

FPIC = `python3 -m pybind11 --includes`
INCLUDE = `python3-config --includes`
CXXFLAGS = -O3 -Wl,--no-as-needed -shared -std=c++17 -fPIC -I${INCLUDE_DIRS} ${MKLFLAGS} -ldl -lpthread -lm $(INCLUDE)

SOFILE = _matrix${shell python3-config --extension-suffix}
SOURCE = matrix.cpp mod.cpp
.PHONY: all
all: ${SOFILE}

${SOFILE}: $(SOURCE)
	${CXX} ${FPIC} $(SOURCE) -o $@ ${CXXFLAGS}

test: ${SOFILE}
	python -m pytest ../validate.py
