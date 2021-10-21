all: build 

build: main.cc line.cc
	g++ -o main $^

run: 
	./main

check: main
	./main > result.txt

clean:
	rm result.txt main