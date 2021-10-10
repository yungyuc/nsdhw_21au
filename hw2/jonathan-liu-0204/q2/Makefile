CFLAGS = -shared --std=c++11 -FPIC -Wall -03
PYFLAGS = `python3 -m pybind11 --includes` -I /usr/include/python3.8
PYCONFIG = `python3-config --extension-suffix`

TARGET = _vector

$(TARGET): get_angle.cpp
	g++ $< -o $@$(PYCONFIG) $(CFLAG) $(PYFLAGS)

.PHONY: test clean

test: $(TARGET)
	python3 -m pytest answer.py

clean:
	rm -rf $(TARGET).*.so
	rm -rf __pycache__