CFLAGS = -Wall
OBJECTS = testafila.o queue.o
CC = gcc

all: testa.e

testa.e: $(OBJECTS)
	$(CC) -o testa.e $(OBJECTS)

testafila.o: testafila.c
	$(CC) $(CFLAGS) -c testafila.c

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

clean:
	-rm -f *.o

purge: clean
	-rm -f testa