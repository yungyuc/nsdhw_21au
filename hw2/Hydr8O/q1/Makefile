.PHONY: clean, run, check


main: main.o
	g++ main.o -o main

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm -rf main *.o

run:
	make

check:
	./main > results.txt

	