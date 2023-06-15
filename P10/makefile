CFLAGS = -Wall
DEBUG = -DDEBUG -g
OBJECTS = pingpong-semaphore.o queue.o ppos_core.o
CC = gcc

all: testa.out

racecond: ppos_core.o pingpong-racecond.o queue.o ppos_core.o
	$(CC) -o testa-racecond.out pingpong-racecond.o queue.o ppos_core.o

debug: ppos_core_debug.o $(OBJECTS)
	$(CC) -o testa.out $(OBJECTS) $(DEBUG)

testa.out: $(OBJECTS)
	$(CC) -o testa.out $(OBJECTS)

pingpong-semaphore.o: pingpong-semaphore.c
	$(CC) $(CFLAGS) -c pingpong-semaphore.c

ppos_core.o: ppos_core.c ppos.h ppos_data.h
	$(CC) $(CFLAGS) -c ppos_core.c

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

ppos_core_debug.o: ppos_core.c ppos.h ppos_data.h
	$(CC) $(CFLAGS) $(DEBUG) -c ppos_core.c 

clean:
	-rm -rf *.o

purge: clean
	-rm -rf *.out
