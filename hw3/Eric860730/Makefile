.PHONY: all test clean

CC = ~/llvm-12.0.0/build/bin/clang++
#CC = clang++
CFLAGS = -Wall -std=c++11 -O3 -m64 -fPIC -shared

MKLROOT = /usr/lib/x86_64-linux-gnu/
#MKLROOT = /opt/intel/mkl
#INCLUDE += -I$(MKLROOT)/include
INCLUDE = -I/usr/include/mkl
#LDFLAGS := \
			   $(MKLROOT)/lib/intel64/libmkl_def.so \
			   $(MKLROOT)/lib/intel64/libmkl_avx2.so \
			   $(MKLROOT)/lib/intel64/libmkl_intel_lp64.so \
			   $(MKLROOT)/lib/intel64/libmkl_sequential.so \
			   $(MKLROOT)/lib/intel64/libmkl_core.so \
			   -lpthread -lm -ldl \
			   -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

LDFLAGS=${MKLROOT}/libmkl_def.so \
		 ${MKLROOT}/libmkl_avx2.so \
		 ${MKLROOT}/libmkl_core.so \
		 ${MKLROOT}/libmkl_intel_lp64.so \
		 ${MKLROOT}/libmkl_sequential.so \
		 -lpthread -lm -ldl \
	     -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5

PYTHON = python3

TARGET = _matrix

all: $(TARGET).*.so

#_GEMM.o: lib/_GEMM.cpp include/_GEMM.hpp
#	$(CC) -o $@ $(CFLAGS) $(INCLUDE) -c $< -lpthread -lm -ldl

$(TARGET).*.so: lib/_GEMM.cpp Matrix_pybind.cpp
	$(CC) $(CFLAGS) $(INCLUDE) `$(PYTHON) -m pybind11 --includes` $^ -o $(TARGET)`$(PYTHON)-config --extension-suffix` -I/usr/include/python3.8 $(LDFLAGS)


test: all
	$(PYTHON) -m pytest -v

benchmark:
	$(PYTHON) benchmark.py

clean:
	$(RM) *.o $(TARGET).*.so performance.txt
