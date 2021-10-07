CXX := g++
CXXFLAGS := -O3 -Wall -shared -std=c++11 -fPIC

all: main.cpp
	$(CXX) $(CXXFLAGS) $(shell python3 -m pybind11 --includes) main.cpp -o _vector$(shell python3-config --extension-suffix)

test:
	pytest vector_test.py

.PHONY: clean
clean:
	rm -rf *.o main *.so __pycache__ .pytest_cache
