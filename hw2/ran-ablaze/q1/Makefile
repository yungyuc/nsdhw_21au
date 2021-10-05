CC = g++
CFLAGS = -Wall -std=gnu++14 -g

EXEC = q1
OUT = result.txt

all: $(EXEC)

$(EXEC): main.cpp
	$(CC) -o $@ $(CFLAGS) $<

.PHONY: run
run: all
	./$(EXEC)

.PHONY: check
check: all
	./$(EXEC) >> $(OUT)

.PHONY: clean
clean:
	rm $(EXEC)
