CFLAGS = -Wall -std=c99
OBJECTS = ppos_core.o
TEST1 = pingpong-tasks1.o 
TEST2 = pingpong-tasks2.o
TEST3 = pingpong-tasks3.o
TESTS = ppos_core.o $(TEST1) $(TEST2) $(TEST3)
CC = gcc
DEBUG = -DDEBUG -g

all: testa.e

testa.e: $(TESTS)
	$(CC) $(CFLAGS) -o testa.e $(TESTS)

pingpong-tasks1.o: pingpong-tasks1.c
	$(CC) $(CFLAGS) -c pingpong-tasks1.c

pingpong-tasks2.o: pingpong-tasks2.c
	$(CC) $(CFLAGS) -c pingpong-tasks2.c

pingpong-tasks3.o: pingpong-tasks3.c
	$(CC) $(CFLAGS) -c pingpong-tasks3.c

debug: $(OBJECTS)
	$(CC) -o testa.e $(OBJECTS) $(DEBUG)

clean:
	-rm -f *.o

purge: clean
	-rm -f testa