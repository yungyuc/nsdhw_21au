CC = g++
CFLAGS = -Wall -Werror -std=c++11 -g
.PHONY: clean run default

default: q1

q1: q1.o line.o
	$(CC) $(CFLAGS) $^ -o $@

q1.o: q1.cpp line.hpp
	$(CC) $(CFLAGS) -c $< -o $@

line.o: line.cpp line.hpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf *.o q1 result.txt

run: default
	@./q1

check: default
	@./q1 > result.txt