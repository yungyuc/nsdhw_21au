.PHONY: clean

INC=/usr/include/mkl
MKLROOT=/usr/lib/x86_64-linux-gnu

SOURCE_FILE_NAME = mod
FILE_NAME = $(SOURCE_FILE_NAME)$(shell python3-config --extension-suffix)

FILE_NAME:
	gcc -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -I$(INC) -lstdc++ -shared -lblas -std=c++17 -fPIC $(shell python3 -m pybind11 --includes) $(shell python3-config --includes) -I/home/egor/miniconda3/envs/nsd/lib/python3.9/site-packages/numpy/core/include $(SOURCE_FILE_NAME).cpp -g -lmkl_intel_lp64 -Wall -lmkl_rt -lmkl_sequential -lmkl_core -lpthread -lm -ldl -o _matrix$(shell python3-config --extension-suffix) -O3



clean:
	rm -rf *.so *.o __pycache__ .pytest_cache