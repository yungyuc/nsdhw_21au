CC = g++
CFLAGS = -Wall -shared -std=c++11 -fPIC
PINC = `python3 -m pybind11 --includes`
SUFFIX = `python3-config --extension-suffix`




all: _vector.*.so

_vector.*.so:_vector.cpp
	$(CC) $(CFLAGS) $(PINC) $< -o _vector$(SUFFIX) -I/usr/include/python3.8

.PHONY: clean
clean:
	rm -rf *.so ./__pycache__ ./.pytest_cache

.PHONY: test
test:all
	python3 -m pytest -vx