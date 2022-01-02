CXX = g++
CXXFLAGS = -O3 -fPIC -Wall -Wextra -std=c++17 -funroll-loops -march=native  -I /usr/include/python3.8 $(shell python3 -m pybind11 --includes)  $(shell python3-config --includes)  -I/usr/include/mkl -I$(shell pwd)
LD_FLAGS = -shared -L/usr/lib/x86_64-linux-gnu/mkl -lblas


SRC_EXT := cpp
TARGET  := _matrix$(shell python3-config --extension-suffix)

SOURCES := $(wildcard *.$(SRC_EXT))

TEST_SCRIPT := test_matrix.py

.PHONY: run clean

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ $(LD_FLAGS) -o $@

test: $(TARGET) $(TEST_SCRIPT)
	python3 -m pytest -v

clean:
	-rm -rf .pytest_cache .mypy_cache __pycache__ performance.txt $(TARGET)

# for debug 
print-%:
	@echo '$*=$($*)'
