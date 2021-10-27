MOD_NAME = _matrix
MOD = $(MOD_NAME)$(shell python3-config --extension-suffix)

CPPFLAGS_DEFAULT = -DNDEBUG

CXXFLAGS_BASE    = -std=c++11 -pedantic -Wall -Wextra -Werror $(shell python3-config --includes)
CXXFLAGS_DEBUG   = -g -fsanitize=address -fsanitize=undefined
CXXFLAGS_DEFAULT = -O3 -funroll-loops -march=native

LDLIBS_BASE  = -lmkl_rt
LDLIBS_DEBUG = -lasan -lubsan

ifdef DEBUG
	CXXFLAGS = $(CXXFLAGS_BASE) $(CXXFLAGS_DEBUG)
	LDLIBS   = $(LDLIBS_BASE) $(LDLIBS_DEBUG)
else
	CPPFLAGS = $(CPPFLAGS_DEFAULT)
	CXXFLAGS = $(CXXFLAGS_BASE) $(CXXFLAGS_DEFAULT)
	LDLIBS   = $(LDLIBS_BASE)
endif

.PHONY: all test clean

all: $(MOD)

$(MOD): build/matrix_pybind.o build/matrix.o build/matrix_mul.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -shared -o $@

build/%.o: %.cpp | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -fPIC -MMD -MF build/$*.d -o $@

build:
	mkdir build

test: $(MOD)
	python3 -m pytest

clean:
	$(RM) -r build $(MOD)

-include build/*.d
