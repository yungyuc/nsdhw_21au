#declare variables
CC = g++
INCLUDE=`python3 -m pybind11 --includes`
POSFIX=`python3-config --extension-suffix`

all: *.so

*.so: vector.cpp pybind11/pybind11.h
	$(CC) -O3 -Wall -shared -std=c++11 -fPIC $(INCLUDE) vector.cpp -o _vector$(POSFIX)

test:
	python3 -m pytest -v

clean:
	rm -f *.so

pybind11/pybind11.h: 
	pip install pybind11