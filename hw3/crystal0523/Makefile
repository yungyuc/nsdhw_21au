CXX =: g++
CFLAG =: -Wall -std=c++11 -O3 -m64 -fPIC -shared

INCLUDES = python3 -m pybind11 --includes
SUFFIX = python3-config --extension-suffix
MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=$MKL_LIB_DIR/libmkl_def.so
MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_avx2.so
MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_core.so
MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_intel_lp64.so
MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_sequential.so

FILE = _matrix

.PHONY: clean test

main: $(FILE)
	$(CXX) $(CFLAG) $(INCLUDES) $(FILE).cpp -o $(FILE) $(SUFFIX) $(MKL_LIBS) -I/usr/include/python3.8

test:
	$(PYTHON) -m pytest

clean:
	rm *.o $(FILE) *.so __pycache__  .pytest_cache performance.txt

