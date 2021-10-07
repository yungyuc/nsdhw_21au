MOD = _vector$(shell python3-config --extension-suffix)

CPPFLAGS_DEFAULT = -DNDEBUG

CXXFLAGS_BASE  = -std=c++11 -pedantic -Wall -Wextra -Werror -MMD $(shell python3-config --includes) -fPIC
CXXFLAGS_DEBUG = -g -fsanitize=address -fsanitize=undefined

LDLIBS_DEBUG = -lasan -lubsan

ifdef DEBUG
	CXXFLAGS = $(CXXFLAGS_BASE) $(CXXFLAGS_DEBUG)
	LDLIBS   = $(LDLIBS_DEBUG)
else
	CPPFLAGS = $(CPPFLAGS_DEFAULT)
	CXXFLAGS = $(CXXFLAGS_BASE)
endif

.PHONY: all test clean

all: $(MOD)

$(MOD): vector_pybind.o vector.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -shared -o $@

test: $(MOD)
	pytest

clean:
	$(RM) *.d *.o $(MOD)

-include *.d
