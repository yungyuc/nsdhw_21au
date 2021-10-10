INCLUDE := `python3 -c "from sysconfig import get_paths;info = get_paths();print(info['include'])"`

all: %.so

%.so:
	g++ -Wall -shared -std=c++14 -fPIC -I ${INCLUDE} _vector.cpp -o _vector`python3-config --extension-suffix`

.PHONY: test
test: %.so
	python3 -m pytest test_vector.py -v

.PHONY: clean
clean:
	rm -rf __pycache__ _vector*.so .pytest_cache

localbuild:
	g++ -Wall -shared -std=c++14 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix`
