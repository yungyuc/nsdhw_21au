all: _vector.so

_vector.so: _vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3-config --includes) _vector.cpp -o _vector$(shell python3-config --extension-suffix)

test: 
	python3 -m pytest -v

clean:
	rm -rf *.so .pytest_cache __pycache__ 