// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.4 -- Janeiro de 2022

// interface do gerente de disco rígido (block device driver)

#ifndef __DISK_MGR__
#define __DISK_MGR__

#include "ppos_data.h"
#include "disk.h"

// estruturas de dados e rotinas de inicializacao e acesso
// a um dispositivo de entrada/saida orientado a blocos,
// tipicamente um disco rigido.

// estrutura que representa um disco no sistema operacional
typedef struct disk_t {
    semaphore_t access; // semaforo de acesso ao disco
    struct disk_request_t *request_queue;
    task_t *waiting_queue;
    int signal;
} disk_t;

// estrutura que representa uma requisicao de operacao em disco
typedef struct disk_request_t {
    struct disk_request_t *prev;
    struct disk_request_t *next;
    task_t *task;
    int operation;
    int block;
    void *buffer;
} disk_request_t;

/*!
 * \brief Inicializa o gerente de disco
 * \param numBlocks Tamanho do disco, em blocos
 * \param blockSize Tamanho de cada bloco do disco, em bytes
 * \return -1 em erro ou 0 em sucesso
 */
int disk_mgr_init(int *numBlocks, int *blockSize);

/*!
 * \brief Leitura de um bloco, do disco para o buffer
 * \param block Bloco a ser lido
 * \param buffer Buffer de dados
 * \return -1 em erro ou 0 em sucesso
 */
int disk_block_read(int block, void *buffer);

/*!
 * \brief Escrita de um bloco, do buffer para o disco
 * \param block Bloco a ser escrito
 * \param buffer Buffer de dados
 * \return -1 em erro ou 0 em sucesso
 */
int disk_block_write(int block, void *buffer);

#endif
