// Vinicius Mioto GRR20203931

#include "ppos_disk.h"
#include "disk.h"
#include "ppos.h"
#include "queue.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

// the task that will be the disk driver
task_t diskDriverTask;

// the disk
disk_t disk;
task_t diskDriverTask;

// the action for the SIGUSR1 signal
struct sigaction actionDisk;

void suspend_disk_driver() {
    // suspend the disk driver task
    diskDriverTask.status = SUSPENDED;
    queue_remove((queue_t **)&ready_tasks, (queue_t *)&diskDriverTask);
    queue_append((queue_t **)&sleeping_tasks, (queue_t *)&diskDriverTask);
}

void diskDriverBody(void *args) {
    disk_request_t *request;

    while (true) {
        sem_down(&disk.access);

        // if the request was acceded, then remove the task from the waiting
        // queue and remove the request from the queue and free it
        if (disk.signal == true) {
#ifdef DEBUG
            printf("disckDriverBody: task resume... \n");
#endif
            task_resume((task_t *)request->task, (task_t **)&disk.waiting_queue);
            queue_remove((queue_t **)&disk.request_queue, (queue_t *)request);

            free(request);
            disk.signal = false;
        }

        int disk_status = disk_cmd(DISK_CMD_STATUS, 0, 0);

        // if the disk is not busy, then get the next request from the queue
        if (disk_status == DISK_STATUS_IDLE && disk.request_queue) {
            request = disk.request_queue;

            disk_cmd(request->operation, request->block, request->buffer);
        }

        // if there is no request in the queue, then the disk is not idle
        if (!disk.request_queue && !disk.waiting_queue) {
            queue_remove((queue_t **)&ready_tasks, (queue_t *)&diskDriverTask);

            diskDriverTask.status = SUSPENDED;
        } else
            diskDriverTask.status = READY;

        sem_up(&disk.access);

        task_yield();
    }
}

void signal_disk_handler() {
    if (diskDriverTask.status == SUSPENDED) {
#ifdef DEBUG
        printf("signal_disk_handler: task resume... \n");
#endif
        task_resume((task_t *)&diskDriverTask, (task_t **)&sleeping_tasks);
    }

    disk.signal = true;
}

int disk_mgr_init(int *numBlocks, int *blockSize) {

    // register the SIGALRM signal handler
    actionDisk.sa_handler = signal_disk_handler;
    sigemptyset(&actionDisk.sa_mask);
    actionDisk.sa_flags = 0;
    if (sigaction(SIGUSR1, &actionDisk, 0) < 0) {
        perror("Erro em sigaction: ");
        exit(1);
    }

    if (disk_cmd(DISK_CMD_INIT, 0, 0))
        return -1;

    sem_init(&disk.access, 1);

    // initialize the disk driver task structure
    disk.request_queue = NULL;
    disk.waiting_queue = NULL;
    disk.signal = false;

    // create a non user task
    diskDriverTask.type = SYSTEM;
    task_init(&diskDriverTask, diskDriverBody, NULL);

#ifdef DEBUG
    printf("disk_mgr_init: task with %d\n", diskDriverTask.id);
#endif

    // suspend the disk driver task
    suspend_disk_driver();

    // get the disk information
    *numBlocks = disk_cmd(DISK_CMD_DISKSIZE, 0, 0);
    *blockSize = disk_cmd(DISK_CMD_BLOCKSIZE, 0, 0);

    // check if the information was correctly obtained
    if (*blockSize < 0 || *numBlocks < 0)
        return -1;

#ifdef DEBUG
    printf(
        "disk_mgr_init: inicia disco com numBlock = %d e blockSize = %d\n",
        *numBlocks,
        *blockSize);

    printf("\n");
#endif

    return 0;
}

disk_request_t *create_request(int block, void *buffer, int operation) {
    disk_request_t *request = (disk_request_t *)malloc(sizeof(disk_request_t));

    if (!request) {
        fprintf(stderr, "create_request: error while allocating memory");
        exit(-1);
    }

    request->prev = NULL;
    request->next = NULL;
    request->block = block;
    request->buffer = buffer;
    request->task = current_task;
    request->operation = operation;

    return request;
}

int disk_block_read(int block, void *buffer) {
    if (buffer == NULL || block < 0)
        return -1;

    // get the access to the disk to create a request
    sem_down(&disk.access);

    disk_request_t *request = create_request(block, buffer, DISK_CMD_READ);
    queue_append((queue_t **)&disk.request_queue, (queue_t *)request);

    if (diskDriverTask.status == SUSPENDED) {
#ifdef DEBUG
        printf("disk_block_read: task resume... \n");
        print_task_queues();
#endif
        task_resume((task_t *)&diskDriverTask, (task_t **)&sleeping_tasks);
    }

    sem_up(&disk.access);

#ifdef DEBUG
    queue_print("\033[0;32m Disk Waiting Queue \033[0;34m \n", (queue_t *)(queue_t *)disk.waiting_queue, print_elem);
    printf("\033[0;m\n");
#endif

    task_suspend((task_t **)&disk.waiting_queue);
    suspend_disk_driver();

    return 0;
}

int disk_block_write(int block, void *buffer) {
    if (buffer == NULL || block < 0)
        return -1;

    // get the access to the disk to create a request
    sem_down(&disk.access);

    disk_request_t *request = create_request(block, buffer, DISK_CMD_WRITE);
    queue_append((queue_t **)&disk.request_queue, (queue_t *)request);

    if (diskDriverTask.status == SUSPENDED) {
#ifdef DEBUG
        printf("disk_block_write: task resume... \n");
#endif
        task_resume((task_t *)&diskDriverTask, (task_t **)&sleeping_tasks);
    }

    sem_up(&disk.access);

#ifdef DEBUG
    queue_print("\033[0;32m Disk Waiting Queue \033[0;34m \n", (queue_t *)(queue_t *)disk.waiting_queue, print_elem);
    printf("\033[0;m\n");
#endif

    task_suspend((task_t **)&disk.waiting_queue);
    suspend_disk_driver();

    return 0;
}
