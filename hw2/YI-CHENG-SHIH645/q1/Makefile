CC = g++
CFLAGS = -Wall -g

q1: q1.o
	$(CC) $(CFLAGS) -o q1 q1.o

q1.o: q1.cpp
	$(CC) $(CFLAGS) -c q1.cpp

run:
	./q1

check:
	./q1 >> result.txt

clean:
	rm -f *.o q1
