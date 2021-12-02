comp=g++

comp_flags=-O3 -Wall -shared -std=c++17 -fPIC

include_dirs=-I/usr/include/pybind11 \
	     -I/usr/include/python3.8 \
	     -I./

mkl_=-I/usr/include/mkl \
     -I/usr/lib/x86_64-linux-gnu/libmkl_* \
     -lblas \
     -lmkl_intel_lp64 \
     -lmkl_intel_thread \
     -lmkl_core \
     -liomp5 \
     -lpthread \
     -lm

defaults:
	${comp} ${comp_flags} $$(python3 -m pybind --includes) ../mod.cpp matrix.hpp -o _matrix$$(python3-config --extension-suffix) ${include_dirs} ${mkl_}

clean:
	rm -rf *.so
	rm -rf __pycache__
	rm -rf *.txt

test: defaults
	python3 -m pytest test_matrix.py
	python3 test_matrix.py
