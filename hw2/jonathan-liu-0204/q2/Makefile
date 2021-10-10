CXX = g++
CFLAGS =  --std=c++11 -g -Wall -O3
#PYFLAGS = `python3 -m pybind11 --includes` -I /usr/include/python3.8
#PYCONFIG = `python3-config --extension-suffix`

TARGET = _vector

$(TARGET): get_angle.cpp
	$(CXX) $(CFLAGS) -shared -fPIC `python3 -m pybind11 --includes` get_angle.cpp -o $@`python3-config --extension-suffix` `python3-config --includes`

# _vector*.so: _vector.cpp
# 	g++ $(CFLAGS) $(PYFLAGS) -Iextern/pybind11/include _vector.cpp -o _vector$(PYCONFIG)

.PHONY: test clean

test:
	python3 -m pytest answer.py

clean:
	rm -f *.o *.so $(TARGET)
	rm -rf __pycache__