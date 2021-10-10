CXX = g++
CFLAGS = -shared --std=c++11 -fPIC -Wall -Wextra -Werror
PYFLAGS = `python3 -m pybind11 --includes` -I /usr/include/python3.8
SUFFIX = `python3-config --extension-suffix`

TARGET = _vector

.PHONY: default test clean

default: $(TARGET)

$(TARGET): get_angle.cpp
	$(CXX) $< -o $@$(SUFFIX) $(CFLAGS) $(PYFLAGS)

test: $(TARGET)
	python3 -m pytest

clean:
	rm -rf $(TARGET).*.so __pycache__/ .pytest_cache/