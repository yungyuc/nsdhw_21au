CXX = g++
CFLAGS = -shared --std=c++11 -m64 -fPIC -Wall -Wextra -Werror
PY_INC = /usr/include/python3.8
PYFLAGS = `python3 -m pybind11 --includes` -I $(PY_INC)

MKL_INC = /usr/include/mkl
MKL_LIB_DIR = /usr/lib/x86_64-linux-gnu
MKL_LIBS = $(MKL_LIB_DIR)/libmkl_def.so \
		   $(MKL_LIB_DIR)/libmkl_avx2.so \
		   $(MKL_LIB_DIR)/libmkl_core.so \
		   $(MKL_LIB_DIR)/libmkl_intel_lp64.so \
		   $(MKL_LIB_DIR)/libmkl_sequential.so
MKLFLAGS = -I $(MKL_INC) $(MKL_LIBS) -lpthread -lm -ldl \
		   -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

TARGET = _matrix
OBJ_SUFFIX = `python3-config --extension-suffix`
.PHONY: default test clean

default: $(TARGET)

$(TARGET): matrix.cpp matrix.hpp
	$(CXX) $< -o $@$(OBJ_SUFFIX) $(CFLAGS) $(PYFLAGS) $(MKLFLAGS)

test: $(TARGET)
	python3 -m pytest

clean:
	rm -rf $(TARGET).*.so __pycache__/ .pytest_cache/
