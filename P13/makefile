CFLAGS = -Wall -lm -lrt
DEBUG = -DDEBUG -g
OBJECTS = pingpong-disco1.o queue.o ppos_core.o disk.o ppos_disk.o 
CC = gcc

all: testa.out

debug: ppos_core_debug.o ppos_disk_debug.o $(OBJECTS)
	$(CC) -o testa.out $(OBJECTS) $(DEBUG) -lm

testa.out: $(OBJECTS)
	$(CC) -o testa.out $(OBJECTS) -lm

pingpong-disco1.o: pingpong-disco1.c
	$(CC) $(CFLAGS) -c pingpong-disco1.c

ppos_core.o: ppos_core.c ppos.h ppos_data.h
	$(CC) $(CFLAGS) -c ppos_core.c

disk.o: disk.c disk.h
	$(CC) $(CFLAGS) -c disk.c

ppos_disk.o: ppos_disk.c ppos.h ppos_data.h
	$(CC) $(CFLAGS) -c ppos_disk.c

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

ppos_core_debug.o: ppos_core.c ppos.h ppos_data.h
	$(CC) $(CFLAGS) $(DEBUG) -c ppos_core.c 

ppos_disk_debug.o: ppos_disk.c ppos.h ppos_data.h
	$(CC) $(CFLAGS) $(DEBUG) -c ppos_disk.c 

clean:
	-rm -rf *.o

purge: clean
	-rm -rf *.out