.PHONY: all run check clean
CC = g++
CFLAGS = -Wall -std=gnu++14 -g
INCLUDE = -Iinclude

LINEDIR = lib/
SRC := $(shell find $(LINEDIR) -name '*.cpp')

EXEC = line
OUTPUTFILE = result.txt
OBJS = main.cpp \
		$(SRC)

# Substitution reference
OBJS := $(OBJS:%.cpp=%.o)

all: $(EXEC)

# Static pattern rule
%.o: %.cpp
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) -c $<

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(INCLUDE)

run:
	./$(EXEC)

check:
	./$(EXEC) > $(OUTPUTFILE)

clean:
	$(RM) $(OBJS) $(EXEC) $(OUTPUTFILE)

