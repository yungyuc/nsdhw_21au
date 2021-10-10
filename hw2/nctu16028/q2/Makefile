CXX = g++
CFLAGS = -shared --std=c++11 -fPIC -Wall -Wextra -Werror
PYFLAGS = `python3 -m pybind11 --includes` -I /usr/include/python3.8
TARGET = _vector
SUFFIX = `python3-config --extension-suffix`
.PHONY: default test clean

default: $(TARGET)

$(TARGET): vecAngle.cpp
	$(CXX) $< -o $@$(SUFFIX) $(CFLAGS) $(PYFLAGS)

test: $(TARGET)
	python3 -m pytest

clean:
	rm -rf $(TARGET).*.so __pycache__/ .pytest_cache/
