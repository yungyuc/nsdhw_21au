CC = clang++
CFLAGS = -std=c++17 -O3 -Wall -Wextra -fPIC -shared -m64
MKLLIB=-I/usr/include/mkl
PYCONFIG = python3-config --extension-suffix
PYBIND = python3 -m pybind11 --includes
MKLROOT=/usr/lib/x86_64-linux-gnu

MKL_LIBS=${MKLROOT}/libmkl_def.so \
		${MKLROOT}/libmkl_avx2.so \
		${MKLROOT}/libmkl_core.so \
		${MKLROOT}/libmkl_intel_lp64.so \
		${MKLROOT}/libmkl_sequential.so

.PHONY: clean all run test

all: _matrix.*.so

_matrix.*.so: matrix.cpp
	$(CC) -o _matrix`$(PYCONFIG)` $(CFLAGS) $(MKL_LIBS) $(MKLLIB) `$(PYBIND)` $<  -I /usr/include/python3.8 -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm

clean:
	rm -rf *.so __pycache__ .pytest_cache

test: all
	python3 -m pytest -vx
