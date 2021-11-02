CXX = g++ -O3 -funroll-loops -march=native
#CXXFLAGS = -fPIC -Wall -Wextra -std=c++11 $(shell python3 -m pybind11 --includes) -I/opt/intel/mkl/include
CXXFLAGS = -fPIC -Wall -Wextra -std=c++11 -I /usr/include/python3.8  -I/usr/include/mkl
# LD_FLAGS = -shared -L/opt/intel/mkl/lib/intel64
LD_FLAGS = -shared -L/usr/lib/x86_64-linux-gnu/mkl -lblas

SRC_EXT := cpp 
TARGET  := _matrix$(shell python3-config --extension-suffix)

SOURCES := $(wildcard *.$(SRC_EXT)) 
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

TEST_SCRIPT := test_matrix.py

.PHONY: run clean

$(TARGET): $(OBJECTS)
	$(CXX) $(LD_FLAGS) -o $@ $^

%.o: %.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET) $(TEST_SCRIPT)
	python3 -m pytest -v

clean:
	-rm -rf .pytest_cache .mypy_cache __pycache__ performance.txt $(TARGET) $(OBJECTS)

# for debug 
print-%:
	@echo '$*=$($*)'
