.PHONY: clean test

PINC =`python3 -m pybind11 --includes`
SUFFIX =`python3-config --extension-suffix`

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so

OPT=-O3 -m64

all:
	clang++ -std=c++17 ${OPT} -Wall -I/usr/include/mkl ${MKL_LIBS} -shared -fPIC $(PINC) _matrix.cpp -o _matrix$(SUFFIX) -I/usr/include/python3.8 \
	-lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm

test: all
	python3 -m pytest test_matrix.py

clean:
	rm -rf *.so ../__pycache__ ../.pytest_cache ../.vscode