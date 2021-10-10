CXX = g++
CXXFLAGS = -std=c++11 -Wall -O3

TARGET = main
objs = *.o
OUTPUT = result.txt

all:
	g++ main.cpp Line.cpp -o main

run:
	./main

check:
	./main > $(OUTPUT)

clean:
	rm -f $(OUTPUT) main