CXX := g++
CFLAGS := --std=c++11
WFLAGS := -Wall -O3 -Wextra
INCLUDE := -I /usr/include/python3.8
TARGET = _angle.*.so
all: $(TARGET)

.PHONY: clean test

$(TARGET): _angle.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -shared -fPIC `python3 -m pybind11 --includes` _angle.cpp -o _angle`python3-config --extension-suffix` `python3-config --includes`

test: $(TARGET)
	python3 -m pytest test.py -v

clean: 
	rm -rf $(TARGET)  __pycache__
