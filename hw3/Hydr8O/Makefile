.PHONY: clean, test

INC=/usr/include/mkl
MKLROOT=/usr/lib/x86_64-linux-gnu -lblas
SOURCE_FILE_NAME = _matrix
FILE_NAME = $(SOURCE_FILE_NAME)$(shell python3-config --extension-suffix)

FILE_NAME:
	gcc -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -I$(INC) -lstdc++ -shared -std=c++11 -fPIC $(shell python3 -m pybind11 --includes) $(shell python3-config --includes) -I ./pybind11/include $(SOURCE_FILE_NAME).cpp -g -lmkl_intel_lp64 -Wall -lmkl_rt -lmkl_sequential -lmkl_core -lpthread -lm -ldl -o $(SOURCE_FILE_NAME)$(shell python3-config --extension-suffix) -O3



clean:
	rm -rf *.so *.o __pycache__ .pytest_cache

test: FILE_NAME
	python -m pytest ../validate.py