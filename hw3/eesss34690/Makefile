FAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S), Linux)
MKLROOT ?= /usr/lib/x86_64-linux-gnu
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/libmkl_def.${MKLEXT} \
	${MKLROOT}/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/libmkl_core.${MKLEXT} \
	${MKLROOT}/libmkl_avx2.${MKLEXT} \
	-lpthread -lm -ldl

MKLFLAGS := ${CXXFLAGS} \
	-I/usr/include/mkl \
	${MKLLINKLINE}

CXX := g++

PYBIND := -O3 -Wall -shared -std=c++11 \
	-fPIC `python3 -m pybind11 --includes` _matrix.cpp \
	-o _matrix`python3-config --extension-suffix`


target: 
	${CXX} ${PYBIND} ${MKLFLAGS}

.PHONY:clean test
test: target
	pytest

clean:
	rm -rf _matrix*.so performance.txt __pycache__
