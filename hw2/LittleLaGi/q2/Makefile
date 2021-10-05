all:
	g++ -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix`

clean:
	rm -rf *.so ./__pycache__ ./.pytest_cache

test:
	python3 -m pytest -v
