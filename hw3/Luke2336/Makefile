CXX := clang++
CFLAGS := --std=c++11 -O3 -m64
WFLAGS := -Wall -Wextra
MKL_DIR := /usr/lib/x86_64-linux-gnu
MKL_LIBS := ${MKL_DIR}/libmkl_def.so \
		        ${MKL_DIR}/libmkl_avx2.so \
		        ${MKL_DIR}/libmkl_core.so \
		        ${MKL_DIR}/libmkl_intel_lp64.so \
		        ${MKL_DIR}/libmkl_sequential.so
MKL_INCLUDE := /usr/include/mkl
MKL := -I $(MKL_INCLUDE) $(MKL_LIBS) -lblas -lmkl_rt -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl
INCLUDE := -I /usr/include/python3.8
TARGET = _matrix.*.so

all: $(TARGET)

.PHONY: clean test

$(TARGET): _matrix.cpp
	$(CXX) $(CFLAGS) -shared -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` `python3-config --includes` $(MKL)

test: $(TARGET)
	python3 -m pytest test_matrix.py

clean: 
	rm -rf $(TARGET)  __pycache__