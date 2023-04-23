// Vinicius Mioto - GRR20203931 (BCC)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppos.h" // estruturas de dados necessárias
#include "queue.h"

#define STACKSIZE 64 * 1024
#define NEW 0
#define READY 1
#define RUNNING 2
#define SUSPENDED 3
#define TERMINATED 4

task_t *current_task; // ponteiro para a tarefa atual
task_t main_task;     // tarefa main
task_t dispatcher;    // tarefa dispatcher
task_t *ready_tasks;

int t_id = 0;

// contador de tarefas do usuário, não inclui o dispatcher
// será usado para o scheduler
long user_tasks_count;

void print_elem(void *ptr)
{
    task_t *elem = ptr;

    if (!elem)
        return;

    elem->prev ? printf("%d", (elem->prev)->id) : printf("*");
    printf("<%d>", elem->id);
    elem->next ? printf("%d", (elem->next)->id) : printf("*");
}

/*!
    \brief Decide a próxima tarefa a ser executada
    \return ponteiro para a próxima tarefa
*/
task_t *scheduler()
{
    if (ready_tasks == NULL)
        return NULL;

    return ready_tasks;
}

void dispatcher_body()
{
    task_t *next_task;

    while (user_tasks_count > 0)
    {
        next_task = scheduler();

        if (next_task == NULL)
            return;

        next_task->status = RUNNING;

        queue_remove((queue_t **)&ready_tasks, (queue_t *)next_task);
        task_switch(next_task);

        switch (next_task->status)
        {
        case READY:
            queue_append((queue_t **)&ready_tasks, (queue_t *)next_task);
            break;
        case TERMINATED:
            free(next_task->context.uc_stack.ss_sp);
            break;
        default:
            break;
        }
    }

    task_exit(0);
}

/*!
    \brief Inicializa o sistema operacional;
    deve ser chamada no inicio do main()
*/
void ppos_init()
{
    // desativa o buffer da saida padrao (stdout), usado pela função printf
    setvbuf(stdout, 0, _IONBF, 0);

    // cria a tarefa main
    main_task.id = t_id;
    main_task.prev = NULL;
    main_task.next = NULL;

    current_task = &main_task;
    ready_tasks = NULL;

    task_init(&dispatcher, dispatcher_body, NULL);

#ifdef DEBUG
    printf("ppos_init: sistema inicializado\n");
#endif

    user_tasks_count = 0;
}

// gerência de tarefas =========================================================

/*!
    \brief Inicializa uma nova tarefa
    \param task descritor da nova tarefa
    \param start_func função corpo da tarefa
    \param arg argumentos para a tarefa
    \return ID > 0 da tarefa criada ou erro
*/
int task_init(task_t *task, void (*start_func)(void *), void *arg)
{
    if (task == NULL)
    {
        fprintf(stderr, "### ERROR task_init: task nula");

        return -1;
    }

    if (start_func == NULL)
    {
        fprintf(stderr, "### ERROR task_init: start_function nula");

        return -2;
    }

    // salva o contexto da tarefa atual
    getcontext(&task->context);

    // inicializa stack para alocar memória para as pilhas de contexto usadas na criação das tarefas.
    char *stack;
    stack = malloc(STACKSIZE);
    if (stack == NULL)
    {
        fprintf(stderr, "### ERROR task_init: pilha não alocada");

        return -3;
    }

    task->context.uc_link = 0;
    task->context.uc_stack.ss_flags = 0;
    task->context.uc_stack.ss_size = STACKSIZE;
    task->context.uc_stack.ss_sp = stack;

    // configura função e argumentos do contexto da tarefa
    makecontext(&(task->context), (void *)(*start_func), 1, arg);

    // define os valores para inserir na fila
    task->next = NULL;
    task->prev = NULL;
    task->id = ++t_id;
    task->status = READY;

    if (task->id > 1)
        queue_append((queue_t **)&ready_tasks, (queue_t *)task);

#ifdef DEBUG
    printf("task_init: iniciada tarefa %d\n", task->id);
#endif

    user_tasks_count++;

    return t_id;
}

/*!
    \brief Identificador da tarefa corrente (main deve ser 0)
    \return ID da tarefa corrente
*/
int task_id()
{
    return current_task ? current_task->id : 0;
}

/*!
    \brief Termina a tarefa corrente, indicando um
    valor de status encerramento
    \param exit_code (int) valor de status de encerramento
*/
void task_exit(int exit_code)
{
#ifdef DEBUG
    printf("task_exit: tarefa %d sendo encerrada\n", task_id());
#endif

#ifdef DEBUG
    queue_print("-> ", (queue_t *)ready_tasks, print_elem);
#endif

    current_task->status = TERMINATED;
    user_tasks_count--;

    // verifica se tarefa atual é a main
    if (current_task->id == 1) {
        task_switch(&main_task);
        return;
    } else if (current_task->id != 0 && exit_code == 0)
        task_yield();
}

/*!
    \brief Alterna a execução para a tarefa indicada
    \param task descritor da tarefa a ser ativada
    \return 0 se sucesso, < 0 se erro
*/
int task_switch(task_t *task)
{
    if (task == NULL)
    {
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

// operações de escalonamento ==================================================

/*!
    \brief A tarefa atual libera o processador para outra tarefa
*/
void task_yield()
{
    if (current_task->status != TERMINATED)
        current_task->status = READY;

    task_switch(&dispatcher);
}
