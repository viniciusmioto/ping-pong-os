// Vinicius Mioto - GRR20203931 (BCC)

#include <stdio.h>
#include <stdlib.h>
#include "ppos.h"// estruturas de dados necessárias

#define STACKSIZE 64*1024

task_t *current_task; // ponteiro para a tarefa atual
task_t main_task; // tarefa main

int t_id = 0;

/*!
    \brief Inicializa o sistema operacional; 
    deve ser chamada no inicio do main()
*/
void ppos_init () {
    // desativa o buffer da saida padrao (stdout), usado pela função printf
    setvbuf(stdout, 0, _IONBF, 0);

    // cria a tarefa main
    main_task.id = t_id;
    main_task.prev = NULL;
    main_task.next = NULL;

    current_task = &main_task;

    #ifdef DEBUG
    printf("ppos_init: sistema inicializado\n");
    #endif
}

// gerência de tarefas =========================================================

/*!
    \brief Inicializa uma nova tarefa
    \param task descritor da nova tarefa
    \param start_func função corpo da tarefa
    \param arg argumentos para a tarefa
    \return ID > 0 da tarefa criada ou erro
*/
int task_init (task_t *task, void (*start_func)(void *), void *arg) {
    if (task == NULL) {
        fprintf(stderr, "### ERROR task_init: task nula");

        return -1;
    }

    if (start_func == NULL) {
        fprintf(stderr, "### ERROR task_init: start_function nula");

        return -2;
    }

    // salva o contexto da tarefa atual
    getcontext(&task->context);

    // inicializa stack para alocar memória para as pilhas de contexto usadas na criação das tarefas.
    char *stack;
    stack = malloc(STACKSIZE);
    if (stack == NULL) {
        fprintf(stderr, "### ERROR task_init: pilha não alocada");

        return -3;
    }   

    task->context.uc_link = 0;
    task->context.uc_stack.ss_flags = 0;
    task->context.uc_stack.ss_size = STACKSIZE;
    task->context.uc_stack.ss_sp = stack;

    // configura função e argumentos do contexto da tarefa
    makecontext(&(task->context), (void*)(*start_func), 1, arg);

    // define os valores para inserir na fila
    task->next = NULL;
    task->prev = NULL;
    task->id = ++t_id;

    #ifdef DEBUG
    printf("task_init: iniciada tarefa %d\n", task->id);
    #endif

    return t_id;
}			

/*!
    \brief Identificador da tarefa corrente (main deve ser 0)
    \return ID da tarefa corrente
*/
int task_id () {
    return current_task ? current_task->id : 0;
}


/*!
    \brief Termina a tarefa corrente, indicando um 
    valor de status encerramento
    \param exit_code (int) valor de status de encerramento
*/
void task_exit (int exit_code) {
    #ifdef DEBUG
    printf("task_exit: tarefa %d sendo encerrada\n", task_id());
    #endif

    // verifica se tarefa atual é a main
    if (current_task->id != 0)
        task_switch(&main_task);
}

/*!
    \brief Alterna a execução para a tarefa indicada
    \param task descritor da tarefa a ser ativada
    \return 0 se sucesso, < 0 se erro
*/
int task_switch (task_t *task) {
    if (task == NULL) {
        fprintf(stderr, "### ERROR task_switch: task selecionada está nula");
        return -1;
    }

    // verifica se está trocando para a mesma tarefa
    if (task_id() == task->id)
        return 0;

    // ucontext_t current_context = current_task->context;

    task_t *aux_current_task = current_task;
    current_task = task;

    #ifdef DEBUG
    printf("task_switch: trocando contexto %d -> %d\n", aux_current_task->id, task->id);
    #endif

    swapcontext(&(aux_current_task->context), &(task->context));

    return 0;
}

/*!
    \brief Suspende a tarefa atual, transferindo-a da 
    fila de prontas para a fila "queue"
    \param queue fila onde a tarefa corrente será transferida
*/
void task_suspend (task_t **queue) {
    
} 

/*!
    \brief Acorda a tarefa indicada, transferindo-a da 
    fila "queue" para a fila de prontas
    \param task descritor da tarefa a ser acordada
    \param queue fila onde a tarefa está suspensa
*/
void task_resume (task_t *task, task_t **queue) {
    
}

// operações de escalonamento ==================================================

/*!
    \brief A tarefa atual libera o processador para outra tarefa
*/
void task_yield () {

}

/*!
    \brief Define a prioridade estática de uma tarefa (ou a tarefa atual)
    \param task descritor da tarefa
    \param prio nova prioridade estática da tarefa
*/
void task_setprio (task_t *task, int prio) {

}

/*!
    \brief Obtém prioridade estática de uma tarefa (ou a tarefa atual)
    \param task descritor da tarefa
    \return prioridade estática da tarefa
*/
int task_getprio (task_t *task) {
    return 0;
}

// operações de gestão do tempo ================================================

// retorna o relógio atual (em milisegundos)

/*!
    \brief Obtém o relógio atual (em milisegundos)
    \return relógio atual
*/
unsigned int systime () {
    return 0;
}

// suspende a tarefa corrente por t milissegundos

/*!
    \brief Suspende a tarefa corrente por t milissegundos
    \param t tempo de suspensão
*/
void task_sleep (int t) {
    
}

// operações de sincronização ==================================================

/*!
    \brief A tarefa corrente aguarda o encerramento de outra tarefa
    \param task descritor da tarefa a ser aguardada
    \return 0 se sucesso, < 0 se erro
*/
int task_wait (task_t *task) {
    return 0;
}

/*!
    \brief Inicializa um semáforo com valor inicial "value"
    \param s descritor do semáforo
    \param value valor inicial do semáforo
    \return 0 se sucesso, < 0 se erro
*/
int sem_init (semaphore_t *s, int value) ;

/*!
    \brief Requisita o semáforo
    \param s descritor do semáforo
    \return 0 se sucesso, < 0 se erro
*/
int sem_down (semaphore_t *s) {
    return 0;
}

/*!
    \brief Libera o semáforo
    \param s descritor do semáforo
    \return 0 se sucesso, < 0 se erro
*/
int sem_up (semaphore_t *s) {
    return 0;
}

/*!
    \brief Destroi o semáforo, liberando as tarefas bloqueadas
    \param s descritor do semáforo
    \return 0 se sucesso, < 0 se erro
*/
int sem_destroy (semaphore_t *s) {
    return 0;
}

/*!
    \brief Inicializa um mutex (sempre inicialmente livre)
    \param m descritor do mutex
    \return 0 se sucesso, < 0 se erro
*/
int mutex_init (mutex_t *m) {
    return 0;
}

// requisita o mutex

/*!
    \brief Requisita o mutex
    \param m descritor do mutex
    \return 0 se sucesso, < 0 se erro
*/
int mutex_lock (mutex_t *m) {
    return 0;
}

/*!
    \brief Libera o mutex
    \param m descritor do mutex
    \return 0 se sucesso, < 0 se erro
*/
int mutex_unlock (mutex_t *m) {
    return 0;
}

/*!
    \brief Destroi o mutex, liberando as tarefas bloqueadas
    \param m descritor do mutex
    \return 0 se sucesso, < 0 se erro
*/
int mutex_destroy (mutex_t *m) {
    return 0;
}

/*!
    \brief Inicializa uma barreira para N tarefas
    \param b descritor da barreira
    \param N número de tarefas que devem chegar na barreira
    \return 0 se sucesso, < 0 se erro
*/
int barrier_init (barrier_t *b, int N) {
    return 0;
}

/*!
    \brief Espera na barreira
    \param b descritor da barreira
    \return 0 se sucesso, < 0 se erro
*/
int barrier_wait (barrier_t *b) {
    return 0;
}

/*!
    \brief Destroi a barreira, liberando as tarefas bloqueadas
    \param b descritor da barreira
    \return 0 se sucesso, < 0 se erro
*/
int barrier_destroy (barrier_t *b) {
    return 0;
}

// operações de comunicação ====================================================

/*!
    \brief Inicializa uma fila para até max mensagens de size bytes cada
    \param queue descritor da fila
    \param max número máximo de mensagens na fila
    \param size tamanho de cada mensagem
    \return 0 se sucesso, < 0 se erro
*/
int mqueue_init (mqueue_t *queue, int max, int size) {
    return 0;
}

/*!
    \brief Envia uma mensagem para a fila
    \param queue descritor da fila
    \param msg mensagem a ser enviada
    \return 0 se sucesso, < 0 se erro
*/
int mqueue_send (mqueue_t *queue, void *msg) {
    return 0;
}

/*!
    \brief Recebe uma mensagem da fila
    \param queue descritor da fila
    \param msg mensagem a ser recebida
    \return 0 se sucesso, < 0 se erro
*/
int mqueue_recv (mqueue_t *queue, void *msg) {
    return 0;
}

/*!
    \brief Destroi a fila, liberando as tarefas bloqueadas
    \param queue descritor da fila
    \return 0 se sucesso, < 0 se erro
*/
int mqueue_destroy (mqueue_t *queue) {
    return 0;
}

/*!
    \brief Informa o número de mensagens atualmente na fila
    \param queue descritor da fila
    \return número de mensagens na fila
*/
int mqueue_msgs (mqueue_t *queue) {
    return 0;
}