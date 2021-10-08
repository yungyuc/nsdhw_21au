.PHONY: all test clean
CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

PYTHON = python3
SRC = _vector

all: $(SRC).*.so

$(SRC).*.so: $(SRC).cpp
	$(CC) $(CFLAGS) `$(PYTHON) -m pybind11 --includes` $< -o _vector`$(PYTHON)-config --extension-suffix` -I /usr/include/python3.8

test:
	$(PYTHON) -m pytest -v

clean:
	rm *.so
