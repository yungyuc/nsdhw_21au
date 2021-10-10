CFLAGS = -shared --std=c++11 -FPIC -Wall -Wextra -Werror
PYFLAGS = `python3 -m pybind11 --includes` -I /usr/include/python3.8
PYCONFIG = `python3-config --extension-suffix`


# $(TARGET): get_angle.cpp
# 	g++ $< -o $@$(PYCONFIG) $(CFLAG) $(PYFLAGS)

_vector*.so: _vector.cpp
	g++ $(CFLAGS) $(PYFLAGS) -Iextern/pybind11/include _vector.cpp -o _vector$(PYCONFIG)

.PHONY: test clean

test:
	python3 -m pytest -v

clean:
	rm -rf _vector *.so
	rm -rf __pycache__