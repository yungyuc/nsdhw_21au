CXXFLAGS=-O3 -Wall -shared -std=c++11 -fPIC
PYBIND=`python3 -m pybind11 --includes`
PYCONFIG=`python3-config --extension-suffix`
all:
	g++ $(CXXFLAGS) $(PYBIND) _vector.cpp -o _vector$(PYCONFIG)
test:
	python3 -m pytest -v

clean:
	rm -rf .pytest_cache;rm -rf __pycache__;rm *.so;