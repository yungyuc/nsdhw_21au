.PHONY: all test benchmark clean

CC = clang++
CFLAGS = -Wall -std=c++11 -O3 -m64 -fPIC -shared

MKL_ROOT = /usr/lib/x86_64-linux-gnu/
INCLUDEs = -I/usr/include/mkl

PYTHON = python3
FILE = _matrix

LDFLAGS=${MKL_ROOT}/libmkl_def.so \
		 ${MKL_ROOT}/libmkl_avx2.so \
		 ${MKL_ROOT}/libmkl_core.so \
		 ${MKL_ROOT}/libmkl_intel_lp64.so \
		 ${MKL_ROOT}/libmkl_sequential.so \
		 -lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5



all: $(FILE).*.so

$(FILE).*.so: 
	$(CC) $(CFLAGS) $(INCLUDES) `$(PYTHON) -m pybind11 --includes` $^ -o $(FILE)`$(PYTHON)-config --extension-suffix` -I/usr/include/python3.8 $(LDFLAGS)

test: all
	$(PYTHON) -m pytest -v

benchmark:
	$(PYTHON) benchmark.py

clean:
	$(RM) *.o $(FILE).*.so performance.txt