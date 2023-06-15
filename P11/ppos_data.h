// Vinicius Mioto - GRR20203931

// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.5 -- Março de 2023

// Estruturas de dados internas do sistema operacional

#ifndef __PPOS_DATA__
#define __PPOS_DATA__

#include "queue.h"
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ucontext.h> // biblioteca POSIX de trocas de contexto

#define STACKSIZE 64 * 1024

// estados das tarefas ----------
#define NEW 0
#define READY 1
#define RUNNING 2
#define SUSPENDED 3
#define TERMINATED 4

// tipos de tarefas ----------
#define USER 0
#define SYSTEM 1

// Estrutura que define um Task Control Block (TCB)
typedef struct task_t {
    struct task_t *prev, *next;  // ponteiros para usar em filas
    unsigned int id;             // identificador da tarefa
    ucontext_t context;          // contexto armazenado da tarefa
    short status;                // pronta, rodando, suspensa, ...
    short int static_prio;       // prioridade estática da tarefa
    short int dynamic_prio;      // prioridade dinâmica da tarefa
    short type;                  // tipo de tarefa (user, system)
    short unsigned int quantum;  // ticks restantes para a tarefa
    unsigned int activations;    // número de ativações da tarefa
    unsigned int processor_time; // tempo de processador consumido pela tarefa
    unsigned int execution_time; // tempo de vida da tarefa
    struct task_t *wait_queue;   // fila de tarefas que estão esperando a tarefa atual
    unsigned int wake_up_time;   // tempo de despertar da tarefa
                                 // ... (outros campos serão adicionados mais tarde)
} task_t;

// estrutura que define um semáforo
typedef struct
{
    int count;
    task_t *queue;
} semaphore_t;

// estrutura que define um mutex
typedef struct
{
    // preencher quando necessário
} mutex_t;

// estrutura que define uma barreira
typedef struct
{
    // preencher quando necessário
} barrier_t;

// estrutura que define uma fila de mensagens
typedef struct
{
    // preencher quando necessário
} mqueue_t;

#endif
