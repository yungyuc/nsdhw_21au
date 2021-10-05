CXX = g++
CFLAGS = --std=c++11 -Wall -O3
TARGET = main
.PHONY : clean run check

$(TARGET): main.cpp Line.o Line.hpp
	$(CXX) $(CFLAGS) main.cpp Line.o -o $@

Line.o : Line.cpp Line.hpp 
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

run: $(TARGET) 
	./$(TARGET)

check: $(TARGET)
	./$(TARGET) > result.txt