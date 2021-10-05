CXX = g++

q1: line.o linemain.o
	$(CXX) line.o linemain.o -o q1

line.o: Line.cpp Line.h
	$(CXX) -c Line.cpp -o line.o

linemain.o: main.cpp Line.h
	$(CXX) -c main.cpp -o linemain.o

.PHONY: clean
clean:
	rm -rf q1 *.o *.txt

run:
	./q1

check:
	./q1 > result.txt


