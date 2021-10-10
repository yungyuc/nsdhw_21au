CFLAGS = -shared --std=c++11 -Wall -03
PYFLAGS = `python3 -m pybind11 --includes`
PYCONFIG = `python3-config --extension-suffix`
PYINCLUDE = `python3-config --includes`

TARGET = _vector

$(TARGET): get_angle.cpp
	g++ $(CFLAGS) -fPIC $(PYFLAGS) get_angle.cpp -o $@$(PYCONFIG) $(PYINCLUDE)

.PHONY: test clean

test: $(TARGET)
	python3 -m pytest answer.py

clean:
	rm -rf $(TARGET).*.so
	rm -rf __pycache__/ .pytest_cache/