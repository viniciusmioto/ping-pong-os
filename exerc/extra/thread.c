/*
Criação de threads POSIX em UNIX.

Compilar com gcc -Wall thread-create.c -o thread-create -lpthread

Carlos Maziero, DINF/UFPR 2020
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3

int y = 0;

void *threadBody(void *id)
{
  int x = 0;
  sleep(2);
  printf("x: %d, y:%d\n", ++x, ++y);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t thread[NUM_THREADS];
  long status;

  pthread_create(&thread[0], NULL, threadBody, (void *)0);
  pthread_create(&thread[1], NULL, threadBody, (void *)1);

  sleep(1);

  pthread_join(thread[0], (void **)&status);
  pthread_join(thread[1], (void **)&status);

  sleep(1);

  pthread_create(&thread[2], NULL, threadBody, (void *)2);
  pthread_join(thread[2], (void **)&status);

  printf("Main: fim\n");
  pthread_exit(NULL);
}
