CXX = g++
CFLAGS = -shared --std=c++11 -m64 -fPIC -Wall -Wextra
PY_INC = /usr/include/python3.8
PYFLAGS = `python3 -m pybind11 --includes` -I $(PY_INC)

MKL_INC = /usr/include/mkl
MKL_LIB_DIR = /usr/lib/x86_64-linux-gnu
MKLFLAGS = -I $(MKL_INC) -L $(MKL_LIB_DIR)/mkl -lblas

TARGET = _matrix
OBJ_SUFFIX = `python3-config --extension-suffix`
.PHONY: default clean

default: $(TARGET)

$(TARGET): matrix.cpp
	$(CXX) $< -o $@$(OBJ_SUFFIX) $(CFLAGS) $(PYFLAGS) $(MKLFLAGS)

clean:
	rm -rf $(TARGET).*.so ../__pycache__/ ../.pytest_cache/
