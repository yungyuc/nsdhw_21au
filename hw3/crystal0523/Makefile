.PHONY: all test benchmark clean

CC = clang++
CFLAGS = -Wall -std=c++11 -O3 -m64 -fPIC -shared
INCLUDES =`python3 -m pybind11 --includes`
SUFFIX =`python3-config --extension-suffix`

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so


PYTHON = python3

all:
	$(CC) ${CFLAGS} -I/usr/include/mkl ${MKL_LIBS} $(INCLUDES) _matrix.cpp -o _matrix$(SUFFIX) -I/usr/include/python3.8 \
	-lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm

test: all
	$(PYTHON) -m pytest test_matrix.py

benchmark:
	$(PYTHON) benchmark.py

clean:
	rm -rf *.o _matrix.*.so performance.txt