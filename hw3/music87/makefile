CXX = g++
TARGET = _matrix
CFLAG = -O3 -Wall -shared -std=c++11 -fPIC
PYINCLUDE = `python3-config --includes` -Iextern/pybind11/include
PYSUFFIX = `python3-config --extension-suffix`
MKLINCLUDE = -I/usr/include/mkl
MKLLIBS = -L/usr/lib/x86_64-linux-gnu/mkl -lblas
TESTFILE = ./validate.py #unit_test.py

.PHONY: run check clean

all: $(TARGET)

$(TARGET): $(TARGET).cpp 
	$(CXX) $(CFLAG) $(PYINCLUDE) $(MKLINCLUDE) $< -o $@$(PYSUFFIX) $(MKLLIBS)

#run: $(TARGET).cpp
#	g++ -O3 -Wall -std=c++11 -fPIC $(PYINCLUDE) $(MKLINCLUDE) $< -o $(TARGET) $(MKLLIBS) 
#	./$(TARGET)
	

test: $(TARGET) 
	python3 -m pytest -v -q $(TESTFILE)

clean:
	rm -rf *.so .pytest_cache __pycache__ 
