CXX = clang++

PYTHON = python3
NUMPY_PATH := $(shell ${PYTHON} -c "import numpy; print(numpy.get_include())")

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	MKLROOT ?= /opt/intel/mkl
	MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
	MKLROOT ?= ${HOME}/opt/conda
	MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
    ${MKLROOT}/lib/libmkl_def.${MKLEXT} \
    ${MKLROOT}/lib/libmkl_avx2.${MKLEXT} \
    ${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
    ${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
    ${MKLROOT}/lib/libmkl_core.${MKLEXT} \
    -lpthread -lm -ldl

CXXFLAGS := ${CXXFLAGS} \
    -std=c++17 -O3 -g -m64 \
	-fsized-deallocation \
	-ftree-vectorize -funroll-loops -ffast-math \
    -I${MKLROOT}/include \
	-I${NUMPY_PATH} \
	-I./ \
    ${MKLLINKLINE} -shared -fPIC

BIN := _matrix

.PHONY: default
default: ${BIN}.*.so

${BIN}.*.so: matrix.hpp
	$(CXX) $(CXXFLAGS) `$(PYTHON) -m pybind11 --includes` ../mod.cpp -o $(BIN)`$(PYTHON)-config --extension-suffix`

.PHONY: clean
clean:
	$(RM) ${BIN} *.so