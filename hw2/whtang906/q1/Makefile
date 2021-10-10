.PHONY: clean run check


main: main.o Line.o
	g++ main.o Line.o -o main

main.o: main.cpp
	g++ -c main.cpp -o main.o

Line.o: Line.cpp 
	g++ -c Line.cpp -o Line.o

clean:
	rm -rf main *.o result.txt

run:
	make

check:
	./main > results.txt