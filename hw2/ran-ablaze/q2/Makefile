CC = g++
CFLAGS = -shared -O3 -Wall -std=gnu++14 -g -fPIC

PYTHON = python3
PYBIND = $(PYTHON) -m pybind11 --includes
PYCONFIG = $(PYTHON)-config --extension-suffix

SRC = _vector

all: $(SRC).*.so

$(SRC).*.so: $(SRC).cpp
	$(CC) -o $(SRC)`$(PYCONFIG)` $(CFLAGS) `$(PYBIND)` $< -I /usr/include/python3.8
.PHONY: test
test: all
	pytest

.PHONY: clean
clean:
	rm $(SRC).*.so
