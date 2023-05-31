CFLAGS = -Wall
DEBUG = -DDEBUG -g
OBJECTS = pingpong-sleep.o queue.o ppos_core.o
CC = gcc

all: testa.out

prio: ppos_core.o queue.o ppos_core.o
	$(CC) -o testa-stress.out queue.o ppos_core.o

debug: ppos_core_debug.o $(OBJECTS)
	$(CC) -o testa.out $(OBJECTS) $(DEBUG)

testa.out: $(OBJECTS)
	$(CC) -o testa.out $(OBJECTS)

pingpong-sleep.o: pingpong-sleep.c
	$(CC) $(CFLAGS) -c pingpong-sleep.c

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
