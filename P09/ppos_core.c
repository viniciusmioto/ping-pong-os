// Vinicius Mioto - GRR20203931 (BCC)

#include "ppos.h" // estruturas de dados necessárias

// tarefas do sistema operacional ----------
task_t *current_task;   // ponteiro para a tarefa atual
task_t main_task;       // tarefa main
task_t dispatcher;      // tarefa dispatcher
task_t *ready_tasks;    // fila de tarefas prontas
task_t *sleeping_tasks; // fila de tarefas adormecidas

// temporizador ----------
#define QUANTUM 20 // quantum em milisegundos

struct sigaction action; // tratador de sinal

struct itimerval timer; // inicialização do timer

unsigned int quantum_ticks = 0; // contador de ticks

int t_id = 0; // id da tarefa atual

// contador de tarefas do usuário, não inclui o dispatcher
// será usado para o scheduler
long user_tasks_count;

unsigned int systime() {
    return quantum_ticks;
}

/*!
 * \brief Imprime o elemento de uma fila
 * \param ptr ponteiro para o elemento
 */
void print_elem(void *ptr) {
    task_t *elem = ptr;

    if (!elem)
        return;

    elem->prev ? printf("%d", (elem->prev)->id) : printf("*");
    printf("<%d>", elem->id);
    elem->next ? printf("%d", (elem->next)->id) : printf("*");
}

/*!
 * \brief Imprime as filas de tarefas prontas e suspensas
 */
void print_task_queues() {
    queue_print("\033[0;32m Ready \033[0;34m \n", (queue_t *)ready_tasks, print_elem);
    printf("\033[0;m");
    queue_print("\033[0;32m Suspended \033[0;34m \n", (queue_t *)sleeping_tasks, print_elem);
    printf("\033[0;m");
}

/*!
 * \brief Verifica se há tarefas dormindo que devem ser acordadas
 */
void check_sleeping_tasks() {
    if (sleeping_tasks == NULL) {
#ifdef DEBUG
        printf("\033[0;30m sleeping_tasks is NULL\033[0m\n");
#endif
        return;
    }

    task_t *aux_task = sleeping_tasks, *next_task;

    unsigned int now = systime();
    do {
        // save the next task cause the current task could be removed from the
        // queue
        next_task = aux_task->next;

        if (aux_task->wake_up_time <= now) {
            aux_task->wake_up_time = 0;
            task_resume((task_t *)aux_task, (task_t **)&sleeping_tasks);
        }

        // atualize the current task
        aux_task = next_task;
    } while (sleeping_tasks && next_task != sleeping_tasks);
}

/*!
 * \brief Encontra a tarefa com maior prioridade na fila (menor valor)
 * \param queue ponteiro para a fila de tarefas
 * \return ponteiro para a tarefa com maior prioridade
 * ou NULL se a fila estiver vazia
 */
task_t *find_task_by_prio(task_t *queue) {
    // verifica se a fila está vazia
    if (queue == NULL)
        return NULL;

    task_t *aux = queue;
    task_t *max_prio_task = aux;

    // percorre a fila para encontrar o valor mínimo
    while (aux->next != queue) {
        aux = aux->next;

        if (aux->dynamic_prio < max_prio_task->dynamic_prio)
            max_prio_task = aux;

        // empate de prioridade dinâmica
        if (aux->dynamic_prio == max_prio_task->dynamic_prio) {
            // maior prioridade estática
            if (aux->static_prio < max_prio_task->static_prio)
                max_prio_task = aux;

            // empate de prioridade estática
            if (aux->static_prio == max_prio_task->static_prio)
                // menor id
                if (aux->id < max_prio_task->id)
                    max_prio_task = aux;
        }
    }

    // reajusta a prioridades dinâmicas (aging)
    aux = queue;
    while (aux->next != queue) {
        if (aux->id != max_prio_task->id && aux->dynamic_prio > -20)
            aux->dynamic_prio--;

        aux = aux->next;
    }

    max_prio_task->dynamic_prio = max_prio_task->static_prio;

    return max_prio_task;
}

/*!
 * \brief Decide a próxima tarefa a ser executada
 * \return ponteiro para a próxima tarefa
 */
task_t *scheduler() {
    if (ready_tasks == NULL && sleeping_tasks == NULL)
        return NULL;

    return find_task_by_prio(ready_tasks);
}

/*!
 * \brief Despachante do sistema operacional. Realiza as trocas de contexto.
 */
void dispatcher_body() {
    task_t *next_task;

    // enaquanto houver tarefa de usuário
    while (queue_size((queue_t *)sleeping_tasks) > 0 ||
           queue_size((queue_t *)ready_tasks) > 0) {
        // próxima tarefa decidida pelo scheduler
        next_task = scheduler();

        if (next_task != NULL) {

            // #ifdef DEBUG
            //             printf("\033[1;30m next_task: ID %d\033[0m\n", next_task->id);
            // #endif

            task_switch(next_task);
            // verifica o status da tarefa
            switch (next_task->status) {
            case TERMINATED:
                queue_remove((queue_t **)&ready_tasks, (queue_t *)next_task);
                free(next_task->context.uc_stack.ss_sp);
                break;
            default:
                break;
            }
        }
        check_sleeping_tasks();
    }
    task_exit(0);
}

/*!
 * \brief Tratador do sinal de timer
 */
static void quantum_handler() {
    current_task->quantum--;
    current_task->processor_time++;
    quantum_ticks++;

    if (current_task->quantum <= 0 && current_task->type == USER) {
        current_task->quantum = QUANTUM;
        task_yield();
    }
}

/*!
 * \brief Inicializa a tarefa main;
 */
void main_setup() {
    main_task.id = t_id;
    main_task.prev = NULL;
    main_task.next = NULL;
    main_task.status = READY;
    main_task.static_prio = 0;
    main_task.dynamic_prio = 0;
    main_task.type = USER;

    current_task = &main_task;
    ready_tasks = NULL;
}

/*!
 * \brief Inicializa o sistema operacional; deve ser chamada no inicio do main()
 */
void ppos_init() {
    // desativa o buffer da saida padrao (stdout), usado pela função printf
    setvbuf(stdout, 0, _IONBF, 0);

    // inicializa a main
    main_setup();

    action.sa_handler = quantum_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGALRM, &action, 0) < 0) {
        perror("\033[0;31m ### ERROR in sigaction: \033[0m\n");
        exit(1);
    }

    // ajusta valores do temporizador
    timer.it_value.tv_usec = 1000;    // primeiro disparo, em micro-segundos
    timer.it_value.tv_sec = 0;        // primeiro disparo, em segundos
    timer.it_interval.tv_usec = 1000; // disparos subsequentes, em micro-segundos
    timer.it_interval.tv_sec = 0;     // disparos subsequentes, em segundos

    // arma o temporizador ITIMER_REAL
    if (setitimer(ITIMER_REAL, &timer, 0) < 0) {
        perror("\033[0;31m ### ERROR in  setitimer: \033[0m\n");
        exit(1);
    }

    queue_append((queue_t **)&ready_tasks, (queue_t *)&main_task);

#ifdef DEBUG
    printf("\033[0;32m Inicializa Main \033[0;m\n");
    print_task_queues();
#endif

    // inicializa o dispatcher
    task_init(&dispatcher, dispatcher_body, NULL);
    task_yield();

    user_tasks_count = 1;
}

/*!
 * \brief Inicializa uma nova tarefa
 * \param task descritor da nova tarefa
 * \param start_func função corpo da tarefa
 * \param arg argumentos para a tarefa
 * \return ID > 0 da tarefa criada ou erro
 */
int task_init(task_t *task, void (*start_func)(void *), void *arg) {
    if (task == NULL) {
        fprintf(stderr, "\033[0;31m ### ERROR task_init: task is NULL \033[0m\n");
        return -1;
    }

    if (start_func == NULL) {
        fprintf(stderr, "\033[0;31m ### ERROR task_init: start_function is NULL \033[0m\n");
        return -2;
    }

    // salva o contexto da tarefa atual
    getcontext(&task->context);

    // inicializa stack para alocar memória para as pilhas
    // de contexto usadas na criação das tarefas.
    char *stack;
    stack = malloc(STACKSIZE);
    if (stack == NULL) {
        fprintf(stderr, "\033[0;31m ### ERROR task_init: could not allocate stack \033[0m\n");

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
    task->static_prio = 0;
    task->dynamic_prio = 0;
    task->quantum = QUANTUM;
    task->activations = 0;
    task->processor_time = 0;
    task->execution_time = quantum_ticks;
    task->wait_queue = NULL;

    // verifica se não é o dispatcher
    if (task->id != 1) {
        task->type = USER;
        queue_append((queue_t **)&ready_tasks, (queue_t *)task);
    }

#ifdef DEBUG
    printf("\033[0;32m task_init: ID %d \033[0m \n", task->id);
#endif

    user_tasks_count++;

#ifdef DEBUG
    print_task_queues();
#endif

    return t_id;
}

/*!
 * \brief Identificador da tarefa corrente (main deve ser 0)
 * \return ID da tarefa corrente
 */
int task_id() {
    return current_task ? current_task->id : 0;
}

/*!
 * \brief Termina a tarefa corrente, indicando um valor de status encerramento
 * \param exit_code (int) valor de status de encerramento
 */
void task_exit(int exit_code) {
    current_task->execution_time = quantum_ticks - current_task->execution_time;

    printf("Task %d exit: execution time %d ms, processor time  %d ms, %d activations\n", task_id(), current_task->execution_time, current_task->processor_time, current_task->activations);

#ifdef DEBUG

#endif

    current_task->status = TERMINATED;
    user_tasks_count--;

    task_t *aux_task;
    while ((aux_task = current_task->wait_queue)) {
#ifdef DEBUG
        printf("task_resume: resume: %d", aux_task->id);
#endif
        task_resume(aux_task, &current_task->wait_queue);
    }

    // verifica se tarefa atual é o dispatcher (id = 1)
    if (current_task->id == 1) {
        task_switch(&main_task);
        return;
    } // verifica se a task atual é a main (id = 0)
    else {
        task_yield();
    }
}

/*!
 * \brief Alterna a execução para a tarefa indicada
 * \param task descritor da tarefa a ser ativada
 * \return 0 se sucesso, < 0 se erro
 */
int task_switch(task_t *task) {
    if (task == NULL) {
        fprintf(stderr, "\033[0;31m ### ERROR task_switch: task selecionada está nula \033[0m\n");
        return -1;
    }

    // verifica se está trocando para a mesma tarefa
    if (task_id() == task->id)
        return 0;

    // ucontext_t current_context = current_task->context;

    task_t *aux_current_task = current_task;
    current_task = task;

    // #ifdef DEBUG
    //     printf("\033[0;32mtask_switch: trocando contexto %d -> %d \033[0m \n\n", aux_current_task->id, task->id);
    // #endif

    task->activations++;

    swapcontext(&(aux_current_task->context), &(task->context));

    return 0;
}

// operações de escalonamento ==================================================

/*!
 * \brief A tarefa atual libera o processador para outra tarefa
 */
void task_yield() {
    task_switch(&dispatcher);
}

/*!
 * \brief Define a prioridade estática de uma tarefa (ou a tarefa atual)
 * \param task descritor da tarefa
 * \param prio nova prioridade estática da tarefa
 */
void task_setprio(task_t *task, int prio) {
    task->dynamic_prio = prio;
    task->static_prio = prio;
}

/*!
 * \brief Obtém prioridade estática de uma tarefa (ou a tarefa atual)
 * \param task descritor da tarefa
 * \return prioridade estática da tarefa
 */
int task_getprio(task_t *task) {
    return task != NULL ? task->dynamic_prio : current_task->dynamic_prio;
}

/*!
 * \brief Suspende a tarefa atual e retorna ao contexto do dispatcher
 * \param queue ponteiro para a fila de tarefas suspensas
 */
void task_suspend(task_t **queue) {
#ifdef DEBUG
    printf("\033[1;36m task_suspend: ID %d \033[0m\n", current_task->id);
#endif

    task_t *aux_task = current_task;

    // verifica se não é o dispatcher
    if (aux_task->id == 1) {
        fprintf(stderr, "\033[0;31m ### ERROR task_suspend: dispatcher cannot be suspended \033[0m \n");
        return;
    }

    if (queue_remove((queue_t **)&ready_tasks, (queue_t *)aux_task) < 0) {
        fprintf(stderr, "\033[0;31m ### ERROR task_suspend: task %d not in ready queue \033[0m\n", aux_task->id);
        return;
    }

    aux_task->status = SUSPENDED;

    if (queue_append((queue_t **)queue, (queue_t *)aux_task) < 0) {
        fprintf(stderr, "\033[0;31m ### ERROR task_suspend: task could not be appended to suspended queue\033[0m\n");
        return;
    }

#ifdef DEBUG
    print_task_queues();
#endif

    task_yield();
}

/*!
 * \brief Reativa uma tarefa e a coloca na fila de prontas
 * \param task descritor da tarefa a ser acordada
 * \param queue ponteiro para a fila de tarefas suspensas
 */
void task_resume(task_t *task, task_t **queue) {
#ifdef DEBUG
    printf("\033[0;36m task_resume: ID %d\n \033[0m", task->id);
#endif

    if (queue_remove((queue_t **)queue, (queue_t *)task) < 0) {
        fprintf(stderr, "\033[0;31m ### ERROR task_resume: task %d \033[0m\n", task->id);
        return;
    }

    if (queue_append((queue_t **)&ready_tasks, (queue_t *)task) < 0) {
        fprintf(stderr, "\033[0;31m ### ERROR task_resume: task %d could not be appended to ready queue \033[0m\n", task->id);
        return;
    }

    task->status = READY;
}

/*!
 * \brief Suspende a tarefa atual para esperar a conclusão de uma tarefa específica
 * \param task descritor da tarefa que deve ser aguardada
 * \return 0 se sucesso, < 0 se erro
 */
int task_wait(task_t *task) {
    if (task == NULL || task->status == TERMINATED)
        return -1;

    task_suspend(&task->wait_queue);

    return task->id;
}

/*!
 * \brief Suspender a tarefa atual por um tempo determinado, sem alterar o estado de outras tarefas
 * \param t tempo em milisegundos
 */
void task_sleep(int t) {
    task_t *aux_task = current_task;
    aux_task->wake_up_time = systime() + t;
    task_suspend(&sleeping_tasks);
}
