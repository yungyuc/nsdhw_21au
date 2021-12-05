XX := g++
CFLAGS := --std=c++17
WFLAGS := -O3 -Wall -fPIC -shared
MKL_DIR := /usr/lib/x86_64-linux-gnu
MKL_LIBS := ${MKL_DIR}
MKL_INCLUDE := /usr/include/mkl
MKL := -L$(MKL_LIBS) -I$(MKL_INCLUDE) -lblas
INCLUDE := -I /usr/include/python3.8 -I/usr/include/pybind11
#INCLUDE:=$(shell python3 -m pybind11 --includes) $(shell python3-config --includes) 
TARGET = _matrix.*.so

all: $(TARGET)

.PHONY: clean test

$(TARGET): _matrix.cpp CustomAllocator.h
	$(CXX) $(CFLAGS) $(WFLAGS) $^ -o _matrix`python3-config --extension-suffix` \
	$(INCLUDE) $(MKL)

test: $(TARGET)
	python3 -m pytest ../validate.py

clean: 
	rm -rf $(TARGET)  __pycache__
