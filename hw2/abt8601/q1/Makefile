CPPFLAGS_DEFAULT = -DNDEBUG

CXXFLAGS_BASE  = -std=c++11 -pedantic -Wall -Wextra -Werror -MMD
CXXFLAGS_DEBUG = -g -fsanitize=address -fsanitize=undefined

LDLIBS_DEBUG = -lasan -lubsan

ifdef DEBUG
	CXXFLAGS = $(CXXFLAGS_BASE) $(CXXFLAGS_DEBUG)
	LDLIBS   = $(LDLIBS_DEBUG)
else
	CPPFLAGS = $(CPPFLAGS_DEFAULT)
	CXXFLAGS = $(CXXFLAGS_BASE)
endif

.PHONY: all run check clean

all: main

main: main.o line.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

run: main
	./main

check: main
	./main > result.txt

clean:
	$(RM) *.d *.o main result.txt

-include *.d
