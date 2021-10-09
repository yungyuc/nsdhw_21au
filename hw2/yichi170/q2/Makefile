CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -O3

TARGET = _vector

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC `python3 -m pybind11 --includes` main.cpp -o $@`python3-config --extension-suffix` `python3-config --includes`

.PHONY: clean test

clean:
	rm -f *.o *.so $(TARGET)
	rm -rf __pycache__

test: $(TARGET)
	python3 -m pytest vec_test.py
