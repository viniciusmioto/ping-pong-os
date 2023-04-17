CFLAGS = -Wall -std=c99
DEBUG = -DDEBUG -g
OBJECTS = pingpong-tasks1.o pingpong-tasks2.o pingpong-tasks3.o


all: ppos_core.o $(OBJECTS)
	gcc -Wall -Wextra ppos_core.o pingpong-tasks1.o -o teste1.e
	gcc -Wall -Wextra ppos_core.o pingpong-tasks2.o -o teste2.e
	gcc -Wall -Wextra ppos_core.o pingpong-tasks3.o -o teste3.e


debug: ppos_core_debug.o $(OBJECTS)
	gcc -Wall -Wextra ppos_core.o pingpong-tasks1.o -o teste1.e
	gcc -Wall -Wextra ppos_core.o pingpong-tasks2.o -o teste2.e
	gcc -Wall -Wextra ppos_core.o pingpong-tasks3.o -o teste3.e


ppos_core.o: ppos_core.c ppos.h ppos_data.h
	gcc $(CFLAGS) -c ppos_core.c


ppos_core_debug.o: ppos_core.c ppos.h ppos_data.h
	gcc $(CFLAGS) $(DEBUG) -c ppos_core.c


pingpong-tasks1.o: pingpong-tasks1.c 
	gcc $(CFLAGS) -c pingpong-tasks1.c


pingpong-tasks2.o: pingpong-tasks2.c 
	gcc $(CFLAGS) -c pingpong-tasks2.c


pingpong-tasks3.o: pingpong-tasks3.c 
	gcc $(CFLAGS) -c pingpong-tasks3.c


diff1:
	diff out1.txt saida-task1.txt


diff2:
	diff out2.txt saida-task2.txt


diff3:
	diff out3.txt saida-task3.txt


clean:
	rm -rf *.o


purge: clean
	rm -rf teste*