CXX =: g++
CFLAG =: -Wall -std=c++11 -O3 -m64 -fPIC -shared


pybind11 = python3 -m pybind11 --includes
suffix = python3-config --extension-suffix
FILE = _vector

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=$MKL_LIB_DIR/libmkl_def.so
MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_avx2.so
MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_core.so
MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_intel_lp64.so
MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_sequential.so



.PHONY: cleam, run, test


all: $(FILE).*.so

$(FILE).*.so: $(FILE).cpp
	$(CXX) $(CXXFLAGS) `$(pybind11)` $< -o $(FILE)`$(suffix)` $(MKL_LIBS) -I /usr/include/python3.8

test: all
	py.test test_matrix.py

clean:
	rm -rf *.so __pycache__  .pytest_cache performance.txt

