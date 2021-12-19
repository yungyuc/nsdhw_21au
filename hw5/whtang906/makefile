CXX = g++

INCLUDE = `python3-config --includes`
NUMPY_INCLUDE = -I$(shell python3 -c "import numpy; print(numpy.get_include())")

# MKL_DIR = /opt/intel/oneapi/mkl/2021.4.0/lib
MKL_DIR = /usr/lib/x86_64-linux-gnu/mkl
# MKL_INCLUDE = /opt/intel/oneapi/mkl/2021.4.0/include
MKL_INCLUDE = /usr/include/mkl
# MKLFLAGS = -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl -lblas
MKLFLAGS = -L /usr/lib/x86_64-linux-gnu/mkl -lblas
# FOR_MAC = -undefined dynamic_lookup
FOR_MAC =

CXXFLAGS = -Wall -O3 -shared -std=c++17 ${FOR_MAC} -fPIC -I${MKL_INCLUDE} ${MKLFLAGS} ${INCLUDE}
FPIC = `python3 -m pybind11 --includes`
SOFILE = _matrix${shell python3-config --extension-suffix}

.PHONY: all
all: ${SOFILE}
${SOFILE}: mod.cpp

	${CXX} ${FPIC} ${NUMPY_INCLUDE} $^ -o $@ ${CXXFLAGS} 

clean:
	rm -rf *.so .pytest_cache __pycache__