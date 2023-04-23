CFLAGS = -Wall -std=c99
DEBUG = -DDEBUG -g
OBJECTS = pingpong-scheduler.o queue.o ppos_core.o
CC = gcc

all: testa.e

debug: ppos_core_debug.o $(OBJECTS)
	$(CC) -o testa.e $(OBJECTS) $(DEBUG)

testa.e: $(OBJECTS)
	$(CC) -o testa.e $(OBJECTS)

pingpong-scheduler.o: pingpong-scheduler.c
	$(CC) $(CFLAGS) -c pingpong-scheduler.c

ppos_core.o: ppos_core.c ppos.h ppos_data.h
	$(CC) $(CFLAGS) -c ppos_core.c

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

ppos_core_debug.o: ppos_core.c ppos.h ppos_data.h
	$(CC) $(CFLAGS) $(DEBUG) -c ppos_core.c 

clean:
	-rm -rf *.o

purge: clean
	-rm -rf testa.e
