##
# NSD HW3
#
# @file
# @version 0.1

CXX = g++
CFLAGS = --std=c++17
WFLAGS = -O3 -Wall -shared
MODULE = _matrix
SOFILE = $(MODULE)$(shell python3-config --extension-suffix)
LBS = -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

.PHONY: all test clean
all: $(SOFILE)

$(SOFILE): matrix.cpp matrix.hpp
	$(CXX) $(WFLAGS) $(CFLAGS) -fPIC $(shell python3-config --includes) -Iextern/pybind11/include matrix.cpp -o $(SOFILE) $(LBS)

test: $(SOFILE)
	python3 -m pytest -v ../validate.py

clean:
	rm -rf $(SOFILE) __pycache__ .pytest_cache performance.txt
