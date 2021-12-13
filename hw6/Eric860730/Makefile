.PHONY: all test benchmark clean

CC = clang++-11
CFLAGS = -Wall -std=c++17 -O3 -m64 -fPIC -shared -Wextra

PYTHON = python3

NUMPY_PATH := $(shell ${PYTHON} -c "import numpy; print(numpy.get_include())")

MKLROOT = /usr/lib/x86_64-linux-gnu
INCLUDE = -I/usr/include/mkl \
		  -I${NUMPY_PATH}

LDFLAGS=${MKLROOT}/libmkl_def.so \
		 ${MKLROOT}/libmkl_avx2.so \
		 ${MKLROOT}/libmkl_core.so \
		 ${MKLROOT}/libmkl_intel_lp64.so \
		 ${MKLROOT}/libmkl_sequential.so \
		 -lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5


TARGET = _matrix

all: $(TARGET).*.so

$(TARGET).*.so: lib/_GEMM.cpp
	$(CC) $(CFLAGS) $(INCLUDE) `$(PYTHON) -m pybind11 --includes` $^ -o $(TARGET)`$(PYTHON)-config --extension-suffix` -I/usr/include/python3.8 $(LDFLAGS)

test: all
	$(PYTHON) -m pytest -v

benchmark:
	$(PYTHON) benchmark.py

clean:
	$(RM) *.o $(TARGET).*.so performance.txt
