// GRR20203931 - Vinícius Mioto
#include "queue.h"
#include <stdio.h>

// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila
int queue_size(queue_t *queue)
{
    int size = 0;

    // check if the queue exists or is not empty
    if (queue != NULL)
    {
        queue_t *aux = queue;
        size = 1;
        // loop through the queue until the last element
        while (aux->next != queue)
        {
            size++;
            aux = aux->next;
        }
    }

    return size;
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
        printf("%s: [] \n", name);
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

    printf("] \n");
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
    {
        fprintf(stderr, "\033[0;31m ### ERROR - queue_append: queue does not exist \033[0m \n");  
        return -1;
    }

    // the element must exist
    if (elem == NULL)
    {
        fprintf(stderr, "\033[0;31m ### ERROR - queue_append: element does not exist \033[0m \n");
        return -2;
    }
    // the element must not be in another queue
    if (elem->next != NULL || elem->prev != NULL)
    {
        fprintf(stderr, "\033[0;31m ### ERROR - queue_append: element is already in a queue \033[0m \n");
        return -3;
    }

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

int search_element(queue_t *queue, queue_t *elem)
{
    // check if the queue exists or is not empty
    if (queue != NULL)
    {
        // check the wanted element once for a unit queue (1 element)
        if ((queue->next == elem->next) && (queue->prev == elem->prev))
            return 0;

        queue_t *aux = queue;

        // loop through the queue until we find the wanted element
        while (aux->next != queue)
        {
            if ((aux->next == elem->next) && (aux->prev == elem->prev))
                return 0;

            aux = aux->next;
        }

        // check if it is the last element
        if ((aux->next == elem->next) && (aux->prev == elem->prev))
            return 0;
    }

    return 1;
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
    // the queue must exist
    if (queue == NULL)
    {
        fprintf(stderr, "\033[0;31m ### ERROR - queue_remove: queue does not exist \033[0m \n");
        return -1;
    }

    // the queue must not be empty
    if (*queue == NULL && queue_size(*queue) == 0)
    {
        fprintf(stderr, "\033[0;31m ### ERROR - queue_remove: queue is empty \033[0m \n");
        return -2;
    }

    // the element must exist
    if (elem == NULL)
    {
        fprintf(stderr, "\033[0;31m ### ERROR - queue_remove: element does not exist \033[0m \n");
        return -3;
    }

    // the element must be in the given queue
    if (search_element(*queue, elem))
    {
        fprintf(stderr, "\033[0;31m ### ERROR - queue_remove: element is not in the given queue \033[0m \n");
        return -4;
    }

    // if it is a unit queue
    if (queue_size(*queue) == 1)
    {
        (*queue)->prev = NULL;
        (*queue)->next = NULL;
        *queue = NULL;
    }

    // if the queue has more than one element
    if (queue_size(*queue) > 1)
    {
        // if the element is the first
        if (elem == *queue)
        {
            (*queue)->next->prev = (*queue)->prev;
            (*queue)->prev->next = (*queue)->next;
            *queue = (*queue)->next;
        }

        // if the element is the last | (*queue)->prev is the last
        else if (elem == (*queue)->prev)
        {
            (((*queue)->prev)->prev)->next = *queue;
            (*queue)->prev = ((*queue)->prev)->prev;
        }

        // element is in the middle
        else
        {
            (elem->prev)->next = elem->next;
            (elem->next)->prev = elem->prev;
        }

        elem->next = NULL;
        elem->prev = NULL;
    }

    return 0;
}
