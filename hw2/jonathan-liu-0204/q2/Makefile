# CXX = g++
# CXXFLAGS =  --std=c++11 -g -Wall -O3
# #PYFLAGS = `python3 -m pybind11 --includes` -I /usr/include/python3.8
# #PYCONFIG = `python3-config --extension-suffix`

# TARGET = _vector

# $(TARGET): get_angle.cpp
# 	$(CXX) $(CXXFLAGS) -shared -fPIC `python3 -m pybind11 --includes` get_angle.cpp -o $@`python3-config --extension-suffix` `python3-config --includes`

# # _vector*.so: _vector.cpp
# # 	g++ $(CFLAGS) $(PYFLAGS) -Iextern/pybind11/include _vector.cpp -o _vector$(PYCONFIG)

# .PHONY: test clean

# test:$(TARGET)
# 	python3 -m pytest answer.py

# clean:
# 	rm -f *.o *.so $(TARGET)
# 	rm -rf __pycache__

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