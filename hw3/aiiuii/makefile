CXX := g++
CFLAGS := --std=c++11
WFLAGS := -O3 -Wall -Wextra -m64 -fPIC -shared
MKL_DIR := /usr/lib/x86_64-linux-gnu
MKL_LIBS := ${MKL_DIR}/lib/intel64
MKL_INCLUDE := /usr/include/mkl
MKL := -L$(MKL_LIBS) -I$(MKL_INCLUDE) -lblas -lmkl_rt -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm
INCLUDE := -I /usr/include/python3.8 -I/usr/include/pybind11
TARGET = _matrix.*.so

all: $(TARGET)

.PHONY: clean test

$(TARGET): _matrix.cpp
	$(CXX) $(CFLAGS) $(WFLAGS) _matrix.cpp -o _matrix`python3-config --extension-suffix` \
	$(INCLUDE) $(MKL)

test: $(TARGET)
	python3 -m pytest unit_test.py

clean: 
	rm -rf $(TARGET)  __pycache__