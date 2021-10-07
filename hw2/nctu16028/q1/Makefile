CXX = g++
CFLAGS = --std=c++11 -Wall -Wextra -Werror
TARGET = main
TXT_FILE = result.txt
.PHONY: default run check clean

default: $(TARGET)

line.o: line.cpp line.hpp
	$(CXX) -c $< -o $@ $(CFLAGS)

main.o: main.cpp line.hpp
	$(CXX) -c $< -o $@ $(CFLAGS)

$(TARGET): line.o main.o
	$(CXX) $^ -o $@ $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

check: $(TARGET)
	./$(TARGET) > $(TXT_FILE)

clean:
	rm -rf *.o $(TARGET) $(TXT_FILE)

