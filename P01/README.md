# Project 01: Contexts

## 1 - Explique o objetivo e os parâmetros de cada uma das quatro funções acima.

A estrutura `ucontext_t` armazena informções sobre contextos 
```c
typedef struct ucontext_t {
    struct ucontext_t *uc_link;
    sigset_t uc_sigmask;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    ...
} ucontext_t;
```

* `int getcontext(ucontext_t *ucp)`:
    * `parâmetro`: endereço de uma variável do tipo **ucontext_t**
    * `ação`: captura o contexto atual e salva na variável **ucp**


* `int setcontext(const ucontext_t *ucp)`:
    * `parâmetro`: endereço de uma variável do tipo **ucontext_t**
    * `ação`: restaura as informações de um contexto previamente salvo em **ucp**

* `int swapcontext(ucontext_t *oucp, const ucontext_t *ucp)`: 
    * `parâmetro`: dois endereços de variáveis do tipo **ucontext_t**
    * `ação`: salva o contexto atual na variavel **oucp** e restaura (ativa) o contexto salvo anteriormente em **ucp**. Basicamente, realiza a troca dos contextos de **oucp** e **ucp**. 

* `void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)`: 
    * `parâmetro`: endereço de uma variável do tipo **ucontext_t**
    * `ação`: ajusta alguns valores internos  do contexto salvo em **ucp**, como por exemplo setar uma funcao que deve ser executada nele.



## 2 - Explique o significado dos campos da estrutura ucontext_t que foram utilizados no código.



## 3 - Explique cada linha do código de contexts.c que chame uma dessas funções ou que manipule estruturas do tipo ucontext_t.

## 4 - Para visualizar melhor as trocas de contexto, desenhe o diagrama de tempo dessa execução.

* description: https://wiki.inf.ufpr.br/maziero/doku.php?id=so:trocas_de_contexto