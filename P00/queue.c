// GRR20203931 - Vinícius Mioto
#include "queue.h"
#include <stdio.h>

// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila
int queue_size(queue_t *queue)
{
    return 0;
}

// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir
void queue_print(char *name, queue_t *queue, void print_elem(void *))
{
    // check if the queue exists or is not empty
    if (queue == NULL)
    {
        printf("%s: []\n", name);
        return;
    }

    printf("%s: [", name);

    queue_t *aux = queue;

    // loop through the queue until the last element
    while (aux->next != queue)
    {
        print_elem(aux);
        printf(" ");
        aux = aux->next;
    }

    // print the last element
    print_elem(aux); 

    printf("]\n");
}

// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
// Retorno: 0 se sucesso, <0 se ocorreu algum erro
int queue_append(queue_t **queue, queue_t *elem)
{
    // the queue must exist
    if (queue == NULL)
        return -1;

    // the element must exist
    if (elem == NULL)
        return -2;

    // the element must not be in another queue
    if (elem->next != NULL || elem->prev != NULL)
        return -3;

    // if the queue is empty, the element will be the first and last
    if (*queue == NULL)
    {
        *queue = elem;
        (*queue)->next = elem;
        (*queue)->prev = elem;
    }
    else
    {
        // the element will be the last
        (*queue)->prev->next = elem;
        elem->prev = (*queue)->prev;
        elem->next = (*queue);
        (*queue)->prev = elem;
    }

    return 0;
}

// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: 0 se sucesso, <0 se ocorreu algum erro
int queue_remove(queue_t **queue, queue_t *elem)
{
    return 0;
}
