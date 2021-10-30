INCLUDES = $(shell python3 -m pybind11 --includes) $(shell python3-config --includes) 
MKL_INC = -I/usr/include/mkl
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas
OUT = _matrix$(shell python3-config --extension-suffix)
TARGET = main

.PHONY: clean test

$(TARGET): _matrix_pyd.cpp _matrix.hpp _matrix.o
	g++ --std=c++11 -Wall -O3 -shared -fPIC $(INCLUDES) $(MKL_INC) $(MKL_LIB) $^ -o $(OUT) -lblas

_matrix.o: _matrix.cpp _matrix.hpp
	g++ --std=c++11 -Wall -O3 -c $(MKL_INC) $< -o $@ 

clean:
	rm -rf __pycache__ .pytest_cache *.o $(TARGET) _matrix*.so performance.txt

test: $(TARGET)
	python -m pytest ../validate.py
