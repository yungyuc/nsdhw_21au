#declare variables
CC = g++
INCLUDE = .

all: main

main: main.o line.o 
	$(CC) -o main main.o line.o
main.o: main.cpp line.h
	$(CC) -I$(INCLUDE) -c main.cpp
line.o: line.cpp line.h
	$(CC) -I$(INCLUDE) -c line.cpp


clean:
	rm -f main.o line.o main


run:
	./main


check:
	./main >> ./result.txt