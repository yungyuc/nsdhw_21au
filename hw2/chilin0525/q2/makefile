all: angle.cc  
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3-config --includes) angle.cc -o _vector$(shell python3-config --extension-suffix)

test: test_angle.py
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache