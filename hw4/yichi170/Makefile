CXX = g++
CXXFLAGS = -std=c++17 -g -O3 -Wall
PYBIND_INC = `python3 -m pybind11 --includes` `python3-config --includes`
MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas
TARGET = _matrix

$(TARGET): main.cpp _matrix.o _matrix.hpp allocator.hpp
	$(CXX) $(CXXFLAGS) -shared -fPIC $(PYBIND_INC) $(MKL_INC) $(MKL_LIB) $^ -o _matrix`python3-config --extension-suffix` -lblas

_matrix.o: _matrix.cpp _matrix.hpp
	$(CXX) $(CXXFLAGS) -fPIC -c $(MKL_INC) $< -o $@

.PHONY: clean test

clean:
	rm -f *.o *.so $(TARGET) performance.txt
	rm -rf __pycache__ .pytest_cache

test: $(TARGET)
	python3 -m pytest ../validate.py
