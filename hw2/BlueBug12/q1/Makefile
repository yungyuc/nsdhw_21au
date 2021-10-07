CXX := g++
CXXFLAGS := -std=c++11 
EXE := main
FILE := result.txt
OBJ := main.o line.o

$(EXE): $(OBJ) 
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	./$(EXE)

check:
	./$(EXE) > $(FILE)

.PHONY: clean
clean:
	rm -rf *.o main $(FILE) 
