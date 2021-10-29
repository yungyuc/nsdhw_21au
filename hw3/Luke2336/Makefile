CXX := g++
CFLAGS := --std=c++11
WFLAGS := -O3 -Wall -Wextra
MKL_DIR := /usr/lib/x86_64-linux-gnu/mkl
MKL_INCLUDE := /usr/include/mkl
MKL := -I $(MKL_INCLUDE) -L $(MKL_DIR) -lblas
INCLUDE := -I /usr/include/python3.8
TARGET = matrix.*.so

all: $(TARGET)

.PHONY: clean test

$(TARGET): matrix.cpp
	$(CXX) $(CFLAGS) -shared -fPIC `python3 -m pybind11 --includes` matrix.cpp -o matrix`python3-config --extension-suffix` `python3-config --includes` $(MKL)

test: $(TARGET)
	python3 -m pytest test_matrix.py

clean: 
	rm -rf $(TARGET)  __pycache__