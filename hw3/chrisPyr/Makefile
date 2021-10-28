CXX=g++
CXXFLAGS=-std=c++17 -O3 -Wall -Wextra -fPIC -shared
MKLLIB=-I/usr/include/mkl
INCLUDE=-Iinclude/
PYTHON=python3
TARGET=main.out
SRC=lib/Mat

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so

.PHONY:clean all test
all: $(TARGET)


$(TARGET): Mat.cpp main.cpp
	$(CXX) -std=c++17 -O3 -Wall -Wextra -fPIC -shared $^ $(MKL_LIBS) -o $@ $(MKLLIB)  -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -lm -I /usr/include/python3.8


test: $(SRC).*.so
	python3 -m pytest -v test.py


$(SRC).*.so: $(SRC).cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(MKL_LIBS) $(MKLLIB) `$(PYTHON) -m pybind11 --includes` $< -o _matrix`$(PYTHON)-config --extension-suffix` -I /usr/include/python3.8 -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm
clean:
	$(RM) -r $(TARGET) *.so *.txt __pycache__* ../__pycache__*
