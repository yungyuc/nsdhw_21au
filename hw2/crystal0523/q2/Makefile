.PHONY: cleam, run, test

CXX := g++
CXXFLAGS := -O3 -Wall -shared -std=gnu++14 -g -fPIC

pybind11 = python -m pybind11 --includes
suffix = python-config --extension-suffix
FILE = _vector

all: $(FILE).*.so

$(FILE).*.so: $(FILE).cpp
	$(CXX) $(CXXFLAGS) `$(pybind11)` $(FILE).cpp -o $(FILE) `$(suffix)`   

test: all
	pytest

clean:
	rm -rf *.so __pycache__  .pytest_cache