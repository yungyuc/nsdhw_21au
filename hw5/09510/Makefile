INCLUDE_DIRS = /usr/include/mkl
MKL_DIR = /usr/lib/x86_64-linux-gnu
MKLFLAGS = ${MKL_DIR}/libmkl_def.so.1 \
	${MKL_DIR}/libmkl_avx2.so.1 \
	${MKL_DIR}/libmkl_core.so.1 \
	${MKL_DIR}/libmkl_intel_lp64.so.1 \
	${MKL_DIR}/libmkl_sequential.so.1

PYTHON_INCLUDE = -I /home/jack/.local/lib/python3.8/site-packages/numpy/core/include -I./

INCLUDE =`python3-config --includes`
CXXFLAGS =${PYTHON_INCLUDE} -I${INCLUDE_DIRS} ${MKLFLAGS} -ldl -lpthread -lm $(INCLUDE) 
OUT = _matrix${shell python3-config --extension-suffix}
cpp_version =-std=c++17
mod=../mod.cpp


.PHONY: all clean test
all: ${OUT}

${OUT}: ${mod} matrix.hpp
	g++ `python3 -m pybind11 --includes` $^ -o $@ -O3 -m64 -Wl,--no-as-needed -shared ${cpp_version} -fPIC ${CXXFLAGS}

clean:
	rm -rf *.so __pycache__ .pytest_cache performance.txt


