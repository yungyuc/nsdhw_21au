CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYCONFIG = python3-config --extension-suffix
PYBIND = python3 -m pybind11 --includes
MKLROOT=/usr/lib/x86_64-linux-gnu
.PHONY: clean all run test

all: _matrix.*.so

_matrix.*.so: _matrix.cpp _matrix.hpp StopWatch.hpp
	dpcpp -o _matrix`$(PYCONFIG)` -fsycl-device-code-split=per_kernel -DMKL_ILP64 -I${MKLROOT}/include $< ${MKLROOT}/lib/intel64/libmkl_sycl.a -Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_ilp64.a ${MKLROOT}/lib/intel64/libmkl_tbb_thread.a ${MKLROOT}/lib/intel64/libmkl_core.a -Wl,--end-group -L${TBBROOT}/lib/intel64/gcc4.8 -ltbb -lsycl -lOpenCL -lpthread -lm -ldl -shared -fPIC `$(PYBIND)`
# gcc -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -I$(INC) -lstdc++ -shared -lblas -std=c++11 -fPIC -I/usr/include/pybind11 $(shell python3-config --includes) $(SOURCE_FILE_NAME).cpp -g -lmkl_intel_lp64 -Wall -lmkl_rt -lmkl_sequential -lmkl_core -lpthread -lm -ldl
# $(CC) -o _matrix`$(PYCONFIG)` $(CFLAGS) $(INCLUDE) `$(PYBIND)` $< 
# matrix: _matrix.cpp _matrix.hpp StopWatch.hpp
# 	dpcpp -o matrix -fsycl-device-code-split=per_kernel -DMKL_ILP64 -I${MKLROOT}/include $< ${MKLROOT}/lib/intel64/libmkl_sycl.a -Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_ilp64.a ${MKLROOT}/lib/intel64/libmkl_tbb_thread.a ${MKLROOT}/lib/intel64/libmkl_core.a -Wl,--end-group -L${TBBROOT}/lib/intel64/gcc4.8 -ltbb -lsycl -lOpenCL -lpthread -lm -ldl

clean:
	rm -rf *.so

test: all
	python3 -m pytest -vx
