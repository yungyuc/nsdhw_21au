CXX := g++
CFLAGS := --std=c++11
WFLAGS := -Wall -O3 -Wextra
INCLUDE := -I /usr/include/python3.8
TARGET = _vector.*.so
all: $(TARGET)

.PHONY: clean test

$(TARGET): _vector.cpp
	$(CXX) $(CFLAGS) -shared -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix` `python3-config --includes`

test: $(TARGET)
	python3 -m pytest test_vector.py

clean: 
	rm -rf $(TARGET)  __pycache__
