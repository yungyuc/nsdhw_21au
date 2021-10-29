.PHONY: all, test, benchmark, clean

CXX = clang++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

INCLUDES = python3 -m pybind11 --includes
SUFFIX = python3-config --extension-suffix
FILE = _matrix
PYTHON = python3


MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
LDFLAGS=${MKL_LIB_DIR}/libmkl_def.so \
		 ${MKL_LIB_DIR}/libmkl_avx2.so \
		 ${MKL_LIB_DIR}/libmkl_core.so \
		 ${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		 ${MKL_LIB_DIR}/libmkl_sequential.so \
		 -lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5


all:
	$(CXX) $(CFLAGS) $(INCLUDES) $< -o $(FILE)$(SUFFIX) -I/usr/include/python3.8 $(LDFLAGS)

test:
	$(PYTHON) pytest test_matrix.py

benchmark:
	$(PYTHON) benchmark.py

clean:
	rm -rf *.so __pycache__  .pytest_cache performance.txt
