// Vinicius Mioto - GRR20203931 (BCC)

#include "ppos.h" // estruturas de dados necessárias

// tarefas do sistema operacional ----------
task_t *current_task;   // ponteiro para a tarefa atual
task_t main_task;       // tarefa main
task_t dispatcher;      // tarefa dispatcher
task_t *ready_tasks;    // fila de tarefas prontas
task_t *sleeping_tasks; // fila de tarefas adormecidas

struct sigaction action;        // tratador de sinal
struct itimerval timer;         // inicialização do timer
unsigned int quantum_ticks = 0; // contador de ticks

int t_id = 0; // id da tarefa atual

// contador de tarefas do usuário, não inclui o dispatcher, usado para o scheduler
long user_tasks_count;

int lock = 0; // variável para controle de seções críticas (semáforos)

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
    if (sleeping_tasks == NULL)
        return;

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
        perror("\033[0;35m ### ERROR in sigaction: \033[0m\n");
        exit(1);
    }

    // ajusta valores do temporizador
    timer.it_value.tv_usec = 1000;    // primeiro disparo, em micro-segundos
    timer.it_value.tv_sec = 0;        // primeiro disparo, em segundos
    timer.it_interval.tv_usec = 1000; // disparos subsequentes, em micro-segundos
    timer.it_interval.tv_sec = 0;     // disparos subsequentes, em segundos

    // arma o temporizador ITIMER_REAL
    if (setitimer(ITIMER_REAL, &timer, 0) < 0) {
        perror("\033[0;35m ### ERROR in  setitimer: \033[0m\n");
        exit(1);
    }

    queue_append((queue_t **)&ready_tasks, (queue_t *)&main_task);

#ifdef DEBUG
    printf("\033[0;32m Inicializa Main ID: %d\033[0;m\n", main_task.id);
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
        fprintf(stderr, "\033[0;35m ### ERROR task_init: task is NULL \033[0m\n");
        return -1;
    }

    if (start_func == NULL) {
        fprintf(stderr, "\033[0;35m ### ERROR task_init: start_function is NULL \033[0m\n");
        return -2;
    }

    // salva o contexto da tarefa atual
    getcontext(&task->context);

    // inicializa stack para alocar memória para as pilhas
    // de contexto usadas na criação das tarefas.
    char *stack;
    stack = malloc(STACKSIZE);
    if (stack == NULL) {
        fprintf(stderr, "\033[0;35m ### ERROR task_init: could not allocate stack \033[0m\n");

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

    print_task_queues();
#endif

    user_tasks_count++;

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

    current_task->status = TERMINATED;
    user_tasks_count--;

    task_t *aux_task;
    while ((aux_task = current_task->wait_queue))
        task_resume(aux_task, &current_task->wait_queue);

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
        fprintf(stderr, "\033[0;35m ### ERROR task_switch: task selecionada está nula \033[0m\n");
        return -1;
    }

    // verifica se está trocando para a mesma tarefa
    if (task_id() == task->id)
        return 0;

    // ucontext_t current_context = current_task->context;

    task_t *aux_current_task = current_task;
    current_task = task;

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
    printf("\033[1;31m ANTES DE SUSPENDER \033[0m\n");
    print_task_queues();
#endif

#ifdef DEBUG
    printf("\033[0;36m task_suspend: ID %d \033[0m\n", current_task->id);
#endif

    task_t *aux_task = current_task;

    // verifica se não é o dispatcher
    if (aux_task->id == 1) {
        fprintf(stderr, "\033[0;35m ### ERROR task_suspend: dispatcher cannot be suspended \033[0m \n");
        return;
    }

    if (queue_remove((queue_t **)&ready_tasks, (queue_t *)aux_task) < 0) {
        fprintf(stderr, "\033[0;35m ### ERROR task_suspend: task %d not in ready queue \033[0m\n", aux_task->id);
        return;
    }

    aux_task->status = SUSPENDED;

    if (queue_append((queue_t **)queue, (queue_t *)aux_task) < 0) {
        fprintf(stderr, "\033[0;35m ### ERROR task_suspend: task could not be appended to suspended tasks queue\033[0m\n");
        return;
    }

#ifdef DEBUG
    printf("\033[1;31m DEPOIS DE SUSPENDER \033[0m\n");
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
    printf("\033[0;36m task_resume: ID %d\n\033[0m", task->id);
#endif

    if (queue_remove((queue_t **)queue, (queue_t *)task) < 0) {
        fprintf(stderr, "\033[0;35m ### ERROR task_resume: task %d \033[0m\n", task->id);
        return;
    }

    if (queue_append((queue_t **)&ready_tasks, (queue_t *)task) < 0) {
        fprintf(stderr, "\033[0;35m ### ERROR task_resume: task %d could not be appended to ready queue \033[0m\n", task->id);
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

#ifdef DEBUG
    printf("\033[0;31m task_wait: TAREFA %d aguarda tarefa %d\n\033[0m", current_task->id, task->id);
#endif

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

/*!
 * \brief Entra em uma seção crítica
 * \param lock ponteiro para a tranca (lock)
 */
void enter_cs(int *lock) {
    while (__sync_fetch_and_or(lock, 1))
        ;
}

/*!
 * \brief Sai de uma seção crítica
 * \param lock ponteiro para a tranca (lock)
 */
void leave_cs(int *lock) {
    (*lock) = 0;
}

/*!
 * \brief Inicializa um semáforo
 * \param s ponteiro para o semáforo
 * \param value valor inicial do semáforo
 * \return 0 se sucesso, < 0 se erro
 */
int sem_init(semaphore_t *s, int value) {
    if (s == NULL) {
        fprintf(stderr, "\033[0;35m ### ERROR sem_init: semaphore is null \033[0m\n");
        return -1;
    }

    s->count = value;
    s->queue = NULL;

    return 0;
}

/*!
 * \brief Operação DOWN em um semáforo
 * \param s ponteiro para o semáforo
 * \return 0 se sucesso, < 0 se erro
 */
int sem_down(semaphore_t *s) {
    if (!s || s->destroyed)
        return -1;

    // enter the critical section
    enter_cs(&lock);

    // decrement the semaphore's count
    s->count--;

    // if the semaphore's count is less than zero, suspend the current task
    task_t *cTask = current_task;
    if (s->count < 0) {
        // remove the task from the ready queue
        queue_remove((queue_t **)&ready_tasks, (queue_t *)cTask);

        // set the task's status to suspended
        cTask->status = SUSPENDED;

        // append the task to the given queue
        queue_append((queue_t **)&s->queue, (queue_t *)cTask);
    }

    // leave the critical section
    leave_cs(&lock);

    // pass the control to the dispatcher
    if (cTask->status == SUSPENDED)
        task_yield();

    return 0;
}

/*!
 * \brief Operação UP em um semáforo
 * \param s ponteiro para o semáforo
 * \return 0 se sucesso, < 0 se erro
 */
int sem_up(semaphore_t *s) {
    if (!s || s->destroyed)
        return -1;

    // enter the critical section
    enter_cs(&lock);

    // increment the semaphore's count
    s->count++;

    // if there is a task suspended in the semaphore's queue, awake it and
    // remove it from the queue
    if (s->count <= 0)
        task_resume((task_t *)s->queue, (task_t **)&s->queue);

    // leave the critical section
    leave_cs(&lock);

    return 0;
}

/*!
 * \brief Destrói um semáforo
 * \param s ponteiro para o semáforo
 * \return 0 se sucesso, < 0 se erro
 */
int sem_destroy(semaphore_t *s) {
    if (!s || s->destroyed)
        return -1;

    // remove all tasks suspended in the semaphore's queue
    while (s->count <= 0)
        sem_up(s);

    s->destroyed = 1;

    return 0;
}

/*!
 * \brief Inicializa fila de mensagens
 * \param queue ponteiro para a fila de mensagens
 * \param max_msgs número máximo de mensagens
 * \param msg_size tamanho máximo de cada mensagem
 * \return 0 se sucesso, < 0 se erro
 */
int mqueue_init(mqueue_t *queue, int max_msgs, int msg_size) {
    queue->msg_size   = msg_size;
    queue->fila = NULL;

    queue->vagas = malloc(sizeof(semaphore_t));
    queue->itens = malloc(sizeof(semaphore_t));
    queue->caixa = malloc(sizeof(semaphore_t));

    if(sem_init(queue->vagas, max_msgs-1)) return -1;
    if(sem_init(queue->itens, 0)) return -1;
    if(sem_init(queue->caixa, 1)) return -1;

    return 0;
}
/*!
 * \brief Envia uma mensagem para a fila de mensagens
 * \param queue ponteiro para a fila de mensagens
 * \param msg ponteiro para a mensagem
 * \return 0 se sucesso, < 0 se erro
 */
int mqueue_send(mqueue_t *queue, void *msg) {
    if (sem_down(queue->vagas))
        return -1;
    if (sem_down(queue->caixa))
        return -1;

    mitem_t *item = malloc(sizeof(mitem_t));
    item->msg = malloc(queue->msg_size);

    if (!item)
        return -1;
    if (!(item->msg))
        return -1;

    memcpy(item->msg, msg, queue->msg_size);

    if (queue_append((queue_t **)&queue->fila, (queue_t *)item))
        return -1;

    if (sem_up(queue->caixa))
        return -1;
    if (sem_up(queue->itens))
        return -1;

    return 0;
}

/*!
 * \brief Recebe uma mensagem da fila de mensagens
 * \param queue ponteiro para a fila de mensagens
 * \param msg ponteiro para a mensagem
 * \return 0 se sucesso, < 0 se erro
 */
int mqueue_recv(mqueue_t *queue, void *msg) {
    if (sem_down(queue->itens))
        return -1;
    if (sem_down(queue->caixa))
        return -1;

    memcpy(msg, queue->fila->msg, queue->msg_size);

    if (queue_remove((queue_t **)&queue->fila, (queue_t *)queue->fila))
        return -1;

    if (sem_up(queue->caixa))
        return -1;
    if (sem_up(queue->vagas))
        return -1;

    return 0;
}

/*!
 * \brief Destrói uma fila de mensagens
 * \param queue ponteiro para a fila de mensagens
 * \return 0 se sucesso, < 0 se erro
 */
int mqueue_destroy(mqueue_t *queue) {
    while (mqueue_msgs(queue) > 0) {
        if (queue_remove((queue_t **)&queue->fila, (queue_t *)queue->fila))
            return -1;
    }

    if (sem_destroy(queue->itens))
        return -1; // tenta destruir sempre mais de uma vez, testar sem->existe
    if (sem_destroy(queue->vagas))
        return -1;
    if (sem_destroy(queue->caixa))
        return -1;

    return 0;
}

/*!
 * \brief Retorna o número de mensagens na fila de mensagens
 * \param queue ponteiro para a fila de mensagens
 * \return número de mensagens na fila de mensagens
 */
int mqueue_msgs(mqueue_t *queue) {
    int size = queue_size((queue_t *)queue->fila);

    if (size == 1 && !(queue->fila))
        return 0;

    return size;
}