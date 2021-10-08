.PHONY: cleam, run, test

CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -fPIC -shared

pybind11 = python3 -m pybind11 --includes
suffix = python-config --extension-suffix
FILE = _vector

all: $(FILE).*.so

$(FILE).*.so: $(FILE).cpp
	$(CXX) $(CXXFLAGS) `$(pybind11)` $< -o $(FILE)`$(suffix)` -I /usr/include/python3.8

test: all
	pytest

clean:
	rm -rf *.so __pycache__  .pytest_cache