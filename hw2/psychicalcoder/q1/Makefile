all: main

main: main.cpp
	g++ main.cpp -o main

.PHONY: run
run: main
	./main

.PHONY: clean
clean:
	rm -rf main result.txt

.PHONY: check
check: main
	./main > result.txt
