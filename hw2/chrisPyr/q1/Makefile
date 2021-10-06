CC = g++
CFLAGS = -Wall -Wextra -std=c++11
INCLUDE = -Iinclude

SOURCE := lib/*.cpp
OBJS := main.o lib/Line.o
EXEC := Line
OUTPUTFILE := result.txt

all: $(EXEC)

$(EXEC): $(OBJS)
	@$(CC) -o Line $(OBJS)

%.o: %.cpp
	@$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

.PHONY: run
run: all
	./$(EXEC)

.PHONY: clean
clean:
	$(RM) $(EXEC) $(OBJS)
.PHONY: check
check:
	./$(EXEC) > $(OUTPUTFILE)

