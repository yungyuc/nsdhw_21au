defaults:
	g++ -O3 -Wall -I/opt/intel/mkl/include -I/usr/include/pybind11 -shared -std=c++11 -fPIC $$(python3 -m pybind --includes) _matrix.cpp -o _matrix$$(python3-config --extension-suffix) -I/usr/include/python3.8 -I/usr/lib/x86_64-linux-gnu/libmkl_* -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm

clean:
	rm -rf *.so
	rm -rf __pycache__
	rm -rf *.txt

test: defaults
	python3 -m pytest test_matrix.py
