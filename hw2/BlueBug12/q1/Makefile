CXX := g++
CXXFLAGS := -std=c++11 
EXE := main
FILE := result.txt

#$(EXE): line.o main.o
#	$(CXX) $(CXXFLAGS) $^ -o $@

#%.o:%.cpp line.hpp
#	$(CXX) $(CXXFLAGS) $< -o $@


$(EXE): main.o line.o
	g++ main.o line.o -o $@

line.o: line.cpp line.hpp
	g++ -c line.cpp -o  $@

main.o: main.cpp line.hpp
	g++ -c main.cpp -o $@

run:
	./main

check:
	./main > $(FILE)

.PHONY: clean
clean:
	rm -rf *.o main $(FILE) 
