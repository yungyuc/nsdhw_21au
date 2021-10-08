CXX := g++
CXXFLAGS := -Wall


main: main.o
	$(CXX) $(CXXFLAGS) main.o -o main

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

run:
	./main echo

check: 
	./main > result.txt

clean:
	rm -f *.o main *.txt

