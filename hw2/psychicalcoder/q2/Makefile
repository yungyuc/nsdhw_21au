INCLUDE := `python3 -c "from sysconfig import get_paths;info = get_paths();print(info['include'])"`

all: %.so

%.so:
	g++ -Wall -shared -std=c++14 -fPIC -I ${INCLUDE} _vector.cpp -o _vector`python3-config --extension-suffix`

.PHONY: test
test: %.so
	pytest test_vector.py

.PHONY: clean
clean:
	rm -rf __pycache__ _vector*.so .pytest_cache
