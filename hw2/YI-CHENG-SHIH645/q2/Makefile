CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYBIND = `python3 -m pybind11 --includes`
EXTENSION = `python3-config --extension-suffix`

q2.so: q2.cpp
	$(CC) $(CFLAGS) $(PYBIND) q2.cpp -o _vector$(EXTENSION) -I /usr/include/python3.8
test:
	python3 -m pytest -v
clean:
	rm -f *.so
