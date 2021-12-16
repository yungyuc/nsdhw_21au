INCLUDE_DIRS = /usr/include/mkl
MKL_DIR = /usr/lib/x86_64-linux-gnu
MKLFLAGS = ${MKL_DIR}/libmkl_def.so.1 \
	${MKL_DIR}/libmkl_avx2.so.1 \
	${MKL_DIR}/libmkl_core.so.1 \
	${MKL_DIR}/libmkl_intel_lp64.so.1 \
	${MKL_DIR}/libmkl_sequential.so.1

INCLUDE = `python3-config --includes`
CXXFLAGS =-I${INCLUDE_DIRS} ${MKLFLAGS} -ldl -lpthread -lm $(INCLUDE)
OUT = _matrix${shell python3-config --extension-suffix}

.PHONY: all clean test
all: ${OUT}

${OUT}: matrix.cpp
	g++ `python3 -m pybind11 --includes` $< -o $@ -O3 -Wl,--no-as-needed -shared -std=c++11 -fPIC ${CXXFLAGS}

clean:
	rm -rf *.so __pycache__ .pytest_cache performance.txt

test: all
	python3 -m pytest -v -s
