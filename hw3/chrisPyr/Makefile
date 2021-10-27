CXX=g++
CXXFLAGS=-std=c++17 -O3 -Wall -Wextra -m64
MKLLIB=-I/usr/include/mkl
TARGET=main.out

MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
MKL_LIBS=${MKL_LIB_DIR}/libmkl_def.so \
		${MKL_LIB_DIR}/libmkl_avx2.so \
		${MKL_LIB_DIR}/libmkl_core.so \
		${MKL_LIB_DIR}/libmkl_intel_lp64.so \
		${MKL_LIB_DIR}/libmkl_sequential.so
.PHONY:clean all test
all: $(TARGET)


$(TARGET): Mat.cpp main.cpp
	$(CXX) $(CXXFLAGS) $(MKLLIB) $(MKL_LIBS) -lblas -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm  $^ -o $@

test: all
	./main.out
clean:
	$(RM) $(TARGET)
