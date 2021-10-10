CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
# -I/Users/yi-cheng/miniforge3/include/python3.9 -I/Users/yi-cheng/miniforge3/lib/python3.9/site-packages/pybind11/include

q2.so: q2.cpp
	$(CC) $(CFLAGS) $(python3 -m pybind11 --includes) q2.cpp -o _vector$(python3-config --extension-suffix) -I /usr/include/python3.8
test:
	python3 -m pytest -v
clean:
	rm -f *.so
