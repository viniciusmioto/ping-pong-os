// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.4 -- Janeiro de 2022
// Teste da implementação de fila genérica queue.c/queue.h.

// ESTE ARQUIVO NÃO DEVE SER MODIFICADO - ELE SERÁ SOBRESCRITO NOS TESTES

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este código foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilação e execução em outros ambientes é responsabilidade do usuário.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

#define N 100

// A estrutura "filaint_t" será usada com as funções de queue.c usando um
// casting para o tipo "queue_t". Isso funciona bem, se os campos iniciais
// de ambas as estruturas forem os mesmos. De acordo com a seção 6.7.2.1 do
// padrão C99: "Within a structure object, the non-bit-ﬁeld members and the
// units in which bit-ﬁelds reside have addresses that increase in the order
// in which they are declared.".

typedef struct filaint_t
{
   struct filaint_t *prev ;  // ptr para usar cast com queue_t
   struct filaint_t *next ;  // ptr para usar cast com queue_t
   int id ;
   // outros campos podem ser acrescidos aqui
} filaint_t ;

filaint_t item[N];
filaint_t *fila0, *fila1, *aux, *final ;
int ret ;

// imprime na tela um elemento da fila (chamada pela função queue_print)
void print_elem (void *ptr)
{
   filaint_t *elem = ptr ;

   if (!elem)
      return ;

   elem->prev ? printf ("%d", elem->prev->id) : printf ("*") ;
   printf ("<%d>", elem->id) ;
   elem->next ? printf ("%d", elem->next->id) : printf ("*") ;
}

//------------------------------------------------------------------------------
int main (int argc, char **argv, char **envp)
{
   int i ;

   // inicializa os N elementos
   for (i=0; i<N; i++)
   {
      item[i].id = i ;
      item[i].prev = NULL ;
      item[i].next = NULL ;
   }

   // PARTE 1: queue_append e queue_size =======================================

   // Teste: inserir N elemementos na fila e verificar a estrutura
   printf ("Testando insercao de %d elementos...\n", N) ;
   fila0 = NULL ;
   for (i=0; i<N; i++)
   {
      assert (queue_size ((queue_t*) fila0) == i) ;
      queue_append ((queue_t **) &fila0, (queue_t*) &item[i]) ;
   }

   queue_print ("Saida gerada  ", (queue_t*) fila0, print_elem) ;

   exit(0) ;
}

