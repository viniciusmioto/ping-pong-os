// Vinicius Mioto GRR20203931

#include "ppos_disk.h"

disk_t disk;
task_t disk_driver_task;
int disk_tasks_count = 0;

struct sigaction actionDisk;

/*!
 * \brief Suspende a tarefa do driver de disco
 */
void suspend_disk_driver() {
    disk_driver_task.status = SUSPENDED;
    queue_remove((queue_t **)&ready_tasks, (queue_t *)&disk_driver_task);
    queue_append((queue_t **)&sleeping_tasks, (queue_t *)&disk_driver_task);
}

/*!
 * \brief Cuida do sinal da tarefa do driver de disco, resume se necessário
 */
static void signal_disk_handler() {
    if (disk_driver_task.status == SUSPENDED) {
#ifdef DEBUG
        printf("signal_disk_handler: task resume... \n");
#endif
        task_resume((task_t *)&disk_driver_task, (task_t **)&sleeping_tasks);
    }

    disk.signal = true;
}

/*!
 \brief Função driver de disco, implementa as ações do driver
 \param args Ponteiro para os argumentos da função
*/
void disk_drive_body(void *args) {
    disk_request_t *request;

    while (true) {
        // obtém o semáforo de acesso ao disco
        sem_down(&disk.access);

        // se foi acordado devido a um sinal do disco
        if (disk.signal == true) {
            // acorda a tarefa cujo pedido foi atendido
#ifdef DEBUG
            printf("disk_drive_body: task resume... \n");
#endif
            task_resume((task_t *)request->task, (task_t **)&disk.waiting_queue);
            queue_remove((queue_t **)&disk.request_queue, (queue_t *)request);

            free(request);
            disk.signal = false;
        }

        int disk_status = disk_cmd(DISK_CMD_STATUS, 0, 0);

        // se o disco estiver livre e houver pedidos de E/S na fila
        if (disk_status == DISK_STATUS_IDLE && disk.request_queue != NULL) {
            // escolhe na fila o pedido a ser atendido, usando FCFS
            request = disk.request_queue;

            // solicita ao disco a operação de E/S, usando disk_cmd()
            if (disk_cmd(request->operation, request->block, request->buffer) < 0) {
#ifdef DEBUG
                fprintf(stderr, "\033[0;35m ### ERROR - disk_driver_body: disk_cmd \033[0m \n");
#endif
            }
        } // verifica fila de tarefas em espera
        else if (ready_tasks == NULL && disk.request_queue == NULL && disk.waiting_queue == NULL) {
            disk_driver_task.status = TERMINATED;
            disk_tasks_count = 0;
#ifdef DEBUG
            print_task_queues();
            printf("disk_drive_body: all tasks were completed... \n");
#endif
            task_exit(0);
        }

        // verifica fila de requisições
        if (disk.request_queue == NULL && disk.waiting_queue == NULL) {
            suspend_disk_driver();

            if (ready_tasks == NULL) {
                queue_remove((queue_t **)&sleeping_tasks, (queue_t *)&disk_driver_task);
                disk_tasks_count = 0;
                disk_driver_task.status = TERMINATED;
            }
        } else
            disk_driver_task.status = READY;

        // libera o semáforo de acesso ao disco
        sem_up(&disk.access);

        // suspende a tarefa corrente (retorna ao dispatcher)
        task_yield();
    }
}

/*!
    \brief Inicializa o gerente de disco
    \param numBlocks Ponteiro para o número de blocos do disco
    \param blockSize Ponteiro para o tamanho do bloco do disco
    \return 0 em caso de sucesso, -1 em caso de erro
*/
int disk_mgr_init(int *numBlocks, int *blockSize) {
    if (disk_cmd(DISK_CMD_INIT, 0, 0))
        return -1;

    // obtém o número de blocos e o tamanho do bloco do disco
    *numBlocks = disk_cmd(DISK_CMD_DISKSIZE, 0, 0);
    *blockSize = disk_cmd(DISK_CMD_BLOCKSIZE, 0, 0);

    if (sem_init(&disk.access, 1) < 0) {
        fprintf(stderr, "\033[0;35m ### ERROR - disk_mgr_init: sem_init \033[0m \n");
        return -1;
    }

    // inicializa as estruturas internas do gerente
    disk.request_queue = NULL;
    disk.waiting_queue = NULL;
    disk_tasks_count = 0;
    disk.signal = false;

    // cria a tarefa do driver de disco
    disk_driver_task.type = SYSTEM;
    if (task_init(&disk_driver_task, disk_drive_body, NULL) < 0) {
        fprintf(stderr, "\033[0;35m ### ERROR - disk_mgr_init: task_init \033[0m \n");
        sem_destroy(&disk.access);
        return -1;
    }

#ifdef DEBUG
    printf("disk_mgr_init: task with %d | requests: %d\n", disk_driver_task.id, disk_tasks_count);
#endif

    // suspende a tarefa do driver de disco até que seja necessário usar o disco
    suspend_disk_driver();

#ifdef DEBUG
    printf(
        "disk_mgr_init: inicia disco com numBlock = %d e blockSize = %d\n",
        *numBlocks,
        *blockSize);

    printf("\n");
#endif

    actionDisk.sa_handler = signal_disk_handler;
    sigemptyset(&actionDisk.sa_mask);
    actionDisk.sa_flags = 0;
    if (sigaction(SIGUSR1, &actionDisk, 0) < 0) {
        fprintf(stderr, "\033[0;35m ### ERROR - disk_mgr_init \033[0m \n");
        exit(1);
    }

    return 0;
}

/*!
    \brief Inicializa uma requisição de E/S
    \param block Número do bloco a ser lido/escrito
    \param buffer Ponteiro para o buffer onde armazenar o bloco lido/escrito
    \param operation Operação de E/S a ser realizada (DISK_CMD_READ ou DISK_CMD_WRITE)
*/
int init_request(int block, void *buffer, int operation) {

#ifdef DEBUG
    printf("init_request: task %d | request: %d\n", current_task->id, disk_tasks_count);
#endif

    disk_request_t *request = (disk_request_t *)malloc(sizeof(disk_request_t));

    if (request == NULL) {
#ifdef DEBUG
        fprintf(stderr, "\033[0;35m ### ERROR - init_request: malloc \033[0m \n");
#endif
        return -1;
    }

    // inicializa os campos da requisição
    request->prev = NULL;
    request->next = NULL;
    request->id = disk_tasks_count++;
    request->task = current_task;
    request->block = block;
    request->buffer = buffer;
    request->operation = (operation == DISK_CMD_WRITE) ? DISK_CMD_WRITE : DISK_CMD_READ;

    if (queue_append((queue_t **)&disk.request_queue, (queue_t *)request) < 0) {
#ifdef DEBUG
        fprintf(stderr, "\033[0;35m ### ERROR - init_request: queue_append \033[0m \n");
#endif
        return -1;
    }

    return 0;
}

/*!
    \brief Solicita uma operação de leitura/escrita ao disco
    \param block Número do bloco a ser lido/escrito
    \param buffer Ponteiro para o buffer onde armazenar o bloco lido/escrito
    \param operation Operação de E/S a ser realizada (DISK_CMD_READ ou DISK_CMD_WRITE)
*/
int disk_block_operation(int block, void *buffer, int operation) {
    // solicita acessa ao disco
    sem_down(&disk.access);

    int cmd;
    switch (operation) {
    case DISK_CMD_READ:
        cmd = DISK_CMD_READ;
        break;
    case DISK_CMD_WRITE:
        cmd = DISK_CMD_WRITE;
        break;
    default:
#ifdef DEBUG
        fprintf(stderr, "\033[0;35m ### ERROR - disk_block_operation: Invalid operation \033[0m \n");
#endif
        sem_up(&disk.access);
        return -1;
    }

    if (init_request(block, buffer, cmd) < 0) {
        sem_up(&disk.access);
#ifdef DEBUG
        fprintf(stderr, "\033[0;35m ### ERROR - disk_block_operation: init_request \033[0m \n");
#endif
        return -1;
    }

    if (disk_driver_task.status == SUSPENDED) {
#ifdef DEBUG
        print_task_queues();
        printf("disk_block_operation: task resume... \n");
#endif
        task_resume((task_t *)&disk_driver_task, (task_t **)&sleeping_tasks);
    }

    // libera o semáforo de acesso ao disco
    sem_up(&disk.access);

#ifdef DEBUG
    queue_print("\033[0;32m Disk Waiting Queue \033[0;34m \n", (queue_t *)(queue_t *)disk.waiting_queue, print_elem);
    printf("\033[0;m\n");
#endif

    // suspende a tarefa corrente (retorna ao dispatcher)
    task_suspend((task_t **)&disk.waiting_queue);
    suspend_disk_driver();

    return 0;
}

int disk_block_read(int block, void *buffer) {
#ifdef DEBUG
    printf("disk_block_read: block = %d\n", block);
#endif

    return disk_block_operation(block, buffer, DISK_CMD_READ);
}

int disk_block_write(int block, void *buffer) {
#ifdef DEBUG
    printf("disk_block_write: block = %d\n", block);
#endif

    return disk_block_operation(block, buffer, DISK_CMD_WRITE);
}
