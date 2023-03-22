// GRR20203931 - Vinícius Mioto
# include "queue.h"

int queue_size (queue_t *queue) {
    return 0;
}

// [9<0>1 0<1>2 1<2>3 2<3>4 3<4>5 4<5>6 5<6>7 6<7>8 7<8>9 8<9>0]
void queue_print (char *name, queue_t *queue, void print_elem (void*) ) {
    if (queue == NULL || !queue) return;
    
    queue_t *aux = queue;

    print_elem(aux);

    while ((aux = aux->next) != queue)  
    {
        print_elem(" ");
        print_elem(aux);
    }
    
    printf("\n");

    return;
}

int queue_append (queue_t **queue, queue_t *elem) {
    // the queue must exist
    if (queue == NULL) return -1;

    // the element must exist
    if (elem == NULL) return -2;

    // the element must not be in another queue
    if (elem->next != NULL || elem->prev != NULL) return -3;

    // if the queue is empty, the element will be the first and last
    if (*queue == NULL) {
        *queue = elem;
        elem->next = elem;
        elem->prev = elem;
    } else {
        // the element will be the last
        elem->next = *queue;
        elem->prev = (*queue)->prev;
        (*queue)->prev->next = elem;
        (*queue)->prev = elem;
    }

    return 0;
}

int queue_remove (queue_t **queue, queue_t *elem) {
    return 0;
}
