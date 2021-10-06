pybind=$(shell python3 -m pybind11 --includes)
suffix=$(shell python3-config --extension-suffix)

all: angle.cc  
	g++ -O3 -Wall -shared -std=c++11 -fPIC $(pybind) angle.cc -o _vector$(suffix)

test: test_angle.py
	python3 test_angle.py

clean:
	rm *.so