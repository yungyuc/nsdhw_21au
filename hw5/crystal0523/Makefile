CXX = g++
CFLAG = -Wall -std=c++17 -O3 -fPIC -shared

pybind11 = `python3 -m pybind11 --includes`
suffix = `python3-config --extension-suffix`
NP_INC = -I$(shell python3 -c "import numpy; print(numpy.get_include())")

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
LDFLAGS=${MKL_LIB_DIR}/libmkl_def.so \
		 ${MKL_LIB_DIR}/libmkl_avx2.so \
		 ${MKL_LIB_DIR}/libmkl_core.so \
		 ${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		 ${MKL_LIB_DIR}/libmkl_sequential.so \
		 -lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

.PHONY: clean, all

all: target

target: ../mod.cpp
	$(CXX) $(CFLAG) -I/usr/include/mkl -I ./ $(NP_INC) $(pybind11) $< -o _matrix$(suffix) -I/usr/include/python3.8 $(LDFLAGS)

clean:
	rm -rf *.so __pycache__  .pytest_cache performance.txt