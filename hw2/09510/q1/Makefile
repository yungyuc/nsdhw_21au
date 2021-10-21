CC = g++
CFLAGS = -Wall -std=c++11 -g

EXEC = q1
TXT = result.txt

all: $(EXEC)

$(EXEC): main.o Line.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.cpp Line.hpp
	$(CC) $(CFLAGS) -c $< -o $@

Line.o: Line.cpp Line.hpp
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: run
run: all
	./$(EXEC)

.PHONY: check
check: all
	./$(EXEC) >> $(TXT)

.PHONY: clean
clean:
	rm -rf $(EXEC) *.o result.txt