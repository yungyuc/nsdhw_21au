CXX=clang++
CXXFLAGS=-O3 -Wall -m64 -shared -std=c++11 -fPIC
PYBIND=`python3 -m pybind11 --includes`
PYCONFIG=`python3-config --extension-suffix`

MKL_LIB_DIR=/opt/intel/mkl/lib/intel64_lin
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so
all: 
	${CXX} $(CXXFLAGS) -I /opt/intel/mkl/include ${MKL_LIBS} $(PYBIND) _matrix.cpp -o _matrix$(PYCONFIG) -I /usr/include/python3.6
test: all
	python3 -m pytest -v -s

clean:
	rm -rf *.so __pycache__ ../.pytest_cache .pytest_cache ../.vscode performance.txt