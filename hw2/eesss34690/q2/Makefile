CXX = g++
STD = c++11
TEST = pytest.py
FPIC = `python3 -m pybind11 --includes`
NAME = `python3-config --extension-suffix`
CPP = _vector
INCLUDE = `python3-config --includes`

q2: $(CPP).cpp
	$(CXX) -O3 -Wall -shared -std=$(STD) -fPIC $(FPIC) $(CPP).cpp -o $(CPP)$(NAME) $(INCLUDE)

clean:
	rm -rf q2 ./__pycache__ *.so

test:
	python $(TEST) 
