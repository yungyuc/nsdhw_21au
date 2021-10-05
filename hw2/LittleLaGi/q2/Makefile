PINC = `python3 -m pybind11 --includes`
SUFFIX = `python3-config --extension-suffix`
INC=$(INSTALL_PREFIX);

all:
	g++ -Wall -shared -std=c++11 -fPIC $(PINC) _vector.cpp -o _vector$(SUFFIX) -I$(INC)

clean:
	rm -rf *.so ./__pycache__ ./.pytest_cache

test:
	python3 -m pytest -v
