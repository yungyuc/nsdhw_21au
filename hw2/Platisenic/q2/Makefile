CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYBIND = python3 -m pybind11 --includes
INCLUDE = -I /usr/include/python3.8
PYCONFIG = python3-config --extension-suffix

.PHONY: clean test default

default: _vector.*.so

_vector.*.so: _vector.cpp
	$(CC) $(CFLAGS) $(INCLUDE) `$(PYBIND)` $< -o _vector`$(PYCONFIG)`

clean:
	@rm *.so

test: default
	@pytest -vx