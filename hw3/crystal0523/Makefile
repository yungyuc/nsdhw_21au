CXX =: g++
CFLAG =: -Wall -std=c++11 -O3 -m64 -fPIC -shared


pybind11 = python3 -m pybind11 --includes
suffix = python3-config --extension-suffix
FILE = _matrix

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
LDFLAGS=${MKL_LIB_DIR}/libmkl_def.so \
		 ${MKL_LIB_DIR}/libmkl_avx2.so \
		 ${MKL_LIB_DIR}/libmkl_core.so \
		 ${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		 ${MKL_LIB_DIR}/libmkl_sequential.so \
		 -lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

.PHONY: cleam, run, test


all: $(FILE).*.so

$(FILE).*.so: $(FILE).cpp
	$(CXX) $(CXXFLAGS) `$(pybind11)` $< -o $(FILE)`$(suffix)` $(MKL_LIBS) -I /usr/include/python3.8 $(LDFLAGS)

test: all
	python3 -m pytest test_matrix.py

benchmark:
	python3 benchmark.py

clean:
	rm -rf *.so __pycache__  .pytest_cache performance.txt