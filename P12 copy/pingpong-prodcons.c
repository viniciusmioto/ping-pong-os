#include "ppos.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int buffer_start = 0;
int buffer_end = 0;

task_t p1, p2, p3, c1, c2;
semaphore_t s_vaga, s_item, s_buffer;


void produtor (void * arg) {

    while (1) {

        task_sleep(1000);
        int item = random() % 100;
        
        sem_down (&s_vaga);

        sem_down (&s_buffer);
        buffer[buffer_end] = item;
        buffer_end = (buffer_end + 1) % BUFFER_SIZE;
        sem_up (&s_buffer);

        sem_up (&s_item);
        printf("%s produziu %d\n", (char *) arg, item);
    }
}

void consumidor(void * arg) {

    while (1) {
        
        int item;
        sem_down(&s_item);

        sem_down(&s_buffer);
        item = buffer[buffer_start];
        buffer_start = (buffer_start + 1) % BUFFER_SIZE;
        sem_up(&s_buffer);

        sem_up(&s_vaga);

        printf("%s consumiu %d\n", (char *) arg, item);
        task_sleep(1000);
    }
}

int main () {

    printf ("main: inicio\n");

    ppos_init ();

    sem_init(&s_vaga, 5);
    sem_init(&s_item, 0);
    sem_init(&s_buffer, 1);

    task_init(&p1, produtor, "p1");
    task_init(&p2, produtor, "     p2");
    task_init(&p3, produtor, "\t  p3");
    task_init(&c1, consumidor, "\t\t\t\tc1");
    task_init(&c2, consumidor, "\t\t\t\t     c2");

    task_wait(&p1);
}