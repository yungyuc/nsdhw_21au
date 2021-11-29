CXX=clang++
CXXFLAGS=-std=c++17 -O3  -m64 -shared -fPIC \
		-I ./ \
		-I /opt/intel/mkl/include \
		-I /home/bwwang/.local/lib/python3.6/site-packages/numpy/core/include \


PYBIND=`python3 -m pybind11 --includes`
PYCONFIG=`python3-config --extension-suffix`

MKL_LIB_DIR=/opt/intel/mkl/lib/intel64_lin
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so

all:
	${CXX} $(CXXFLAGS) $(MKL_LIBS) $(PYBIND) ../mod.cpp -o _matrix$(PYCONFIG) -I /usr/include/python3.6

clean:
	rm -rf *.so __pycache__ ../.pytest_cache .pytest_cache ../.vscode performance.txt

# CXX = clang++

# PYTHON = python3
# NUMPY_PATH := $(shell ${PYTHON} -c "import numpy; print(numpy.get_include())")

# UNAME_S := $(shell uname -s)

# ifeq ($(UNAME_S),Darwin)
# 	MKLROOT ?= /opt/intel/mkl
# 	MKLEXT ?= a
# CXXFLAGS :=
# endif

# ifeq ($(UNAME_S),Linux)
# 	MKLROOT ?= ${HOME}/opt/conda
# 	MKLEXT ?= so
# CXXFLAGS := -Wl,--no-as-needed
# endif

# MKL_LIB_DIR=/opt/intel/mkl/lib/intel64_lin
# MKLLINKLINE=${MKL_LIB_DIR}/libmkl_def.so \
# 		${MKL_LIB_DIR}/libmkl_avx2.so \
# 		${MKL_LIB_DIR}/libmkl_core.so \
# 		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
# 		${MKL_LIB_DIR}/libmkl_sequential.so

# CXXFLAGS := ${CXXFLAGS} \
#     -std=c++17 -O3 -g -m64 \
# 	-fsized-deallocation \
# 	-ftree-vectorize -funroll-loops -ffast-math \
#     -I /opt/intel/mkl/include \
# 	-I${NUMPY_PATH} \
# 	-I./ \
#     ${MKLLINKLINE} -shared -fPIC

# BIN := _matrix

# .PHONY: default
# default: ${BIN}.*.so

# ${BIN}.*.so: matrix.hpp
# 	$(CXX) $(CXXFLAGS) `$(PYTHON) -m pybind11 --includes` ../mod.cpp -o $(BIN)`$(PYTHON)-config --extension-suffix`

# .PHONY: clean
# clean:
# 	$(RM) ${BIN} *.so