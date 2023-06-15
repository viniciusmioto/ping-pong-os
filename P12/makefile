# CFLAGS = -Wall
# DEBUG = -DDEBUG -g
# OBJECTS = pingpong-mqueue.o queue.o ppos_core.o
# CC = gcc

# all: testa.out

# debug: ppos_core_debug.o $(OBJECTS)
# 	$(CC) -o testa.out $(OBJECTS) $(DEBUG) -lm

# testa.out: $(OBJECTS)
# 	$(CC) -o testa.out $(OBJECTS) -lm

# pingpong-mqueue.o: pingpong-mqueue.c
# 	$(CC) $(CFLAGS) -c pingpong-mqueue.c

# ppos_core.o: ppos_core.c ppos.h ppos_data.h
# 	$(CC) $(CFLAGS) -c ppos_core.c

# queue.o: queue.c
# 	$(CC) $(CFLAGS) -c queue.c

# ppos_core_debug.o: ppos_core.c ppos.h ppos_data.h
# 	$(CC) $(CFLAGS) $(DEBUG) -c ppos_core.c 

# clean:
# 	-rm -rf *.o

# purge: clean
# 	-rm -rf *.out


CFLAGS = -Wall
DEBUG = -DDEBUG -g
OBJECTS = pingpong-prodcons.o queue.o ppos_core.o
CC = gcc

all: testa.out

debug: ppos_core_debug.o $(OBJECTS)
	$(CC) -o testa.out $(OBJECTS) $(DEBUG)

testa.out: $(OBJECTS)
	$(CC) -o testa.out $(OBJECTS)

pingpong-prodcons.o: pingpong-prodcons.c
	$(CC) $(CFLAGS) -c pingpong-prodcons.c

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

