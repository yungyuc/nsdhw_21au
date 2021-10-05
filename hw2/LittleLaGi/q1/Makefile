# $@: target
# $^: dependencies
# $*: dependencies without suffix
# $?: dependencies need to be rebuilt
# %: wildcard

CC:=g++
FLAG:=-g -Wall
EXEC:=main.out
OBJ:=main.o Line.o

all: $(OBJ)
	$(CC) $(FLAG) -o $(EXEC) $(OBJ)

%.o: %.cpp
	$(CC) $(FLAG) -o $@ -c $^

clean:
	rm -rf $(OBJ) main.out

run:
	./main.out

check:
	./main.out >> result.txt