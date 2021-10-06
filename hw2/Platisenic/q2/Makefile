CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYBIND = python3 -m pybind11 --includes
PYCONFIG = python3-config --extension-suffix

.PHONY: clean test default

default: cal_vector.*.so

cal_vector.*.so: cal_vector.cpp
	$(CC) $(CFLAGS) `$(PYBIND)` $< -o cal_vector`$(PYCONFIG)`

clean:
	@rm *.so

test:
	@pytest -vx