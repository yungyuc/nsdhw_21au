CXX = g++
TARGET = _vector
CFLAG = -O3 -Wall -shared -std=c++11 -fPIC
PYINCLUDE = `python3 -m pybind11 --includes`
PYSUFFIX = `python3-config --extension-suffix`

.PHONY: run check clean

all: $(TARGET)

$(TARGET): $(TARGET).cpp 
	$(CXX) $(CFLAG) $(PYINCLUDE) $< -o $@$(PYSUFFIX)

test: 
	python3 -m pytest -v

clean:
	rm -rf *.so .pytest_cache __pycache__ 
