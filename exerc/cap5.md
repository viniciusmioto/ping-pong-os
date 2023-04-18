# Exercícios Capítulo 5 

O estado de uma tarefa em um determinado
instante é denominado **contexto**.

O ato de suspender uma tarefa e reativar outra é denominado uma **troca de contexto**.

**Despachante** ou executivo (*dispatcher*) é um conjunto de rotinas que provê armazenamento e recuperação do contexto e a atualização das informações contidas no TCB de cada tarefa. `Mecanismos`

**Escalonador** (*scheduler*) responsável pela escolha da próxima tarefa a receber o processador a cada troca de contexto. Considera influências de diversos fatores, como as prioridades, os tempos de vida e os tempos de processamento restante de cada tarefa. `Política`

## 1 - Explique o que é, para que serve e o que contém um TCB - Task Control Block

TCB é um descritor, ou seja, uma estrutura de dados no núcleo que representa essa tarefa. Nessa estrutura de dados são armazenadas as informações relativas ao seu contexto e os demais dados necessários à sua gerência, como prioridades, estado, etc

## 2 - Desenhe o diagrama de tempo da execução do código a seguir, informe qual a saída do programa na tela (com os valores de x) e calcule a duração aproximada de sua execução.

```c
int main() {
    int x = 0 ;
    fork () ;
    x++ ;
    sleep (5) ;
    wait (0) ;
    fork () ;
    wait (0) ;
    sleep (5) ;
    x++ ;
    printf ("Valor de x: %d\n", x) ;
}
```

![diagram](../img/cap5-ex2.jpg)

## 8 - Considerando as implementações de threads N:1 e 1:1 para o trecho de código a seguir, (a) desenhe os diagramas de execução, (b) informe as durações aproxima- das de execução e (c) indique a saída do programa na tela. Considere a operação sleep() como uma chamada de sistema (syscall). 

## A chamada thread_create cria uma nova thread, thread_exit encerra a thread corrente e thread_join espera o encerramento da thread informada como parâmetro.

```c
int y = 0;

void threadBody
{
  int x = 0;
  sleep(10);
  printf("x: %d, y:%d\n", ++x, ++y);
  thread_exit();
}

main()
{
  thread_create(&tA, threadBody, ...);
  thread_create(&tB, threadBody, ...);
  sleep(1);
  thread_join(&tA);
  thread_join(&tB);
  sleep(1);
  thread_create(&tC, threadBody, ...);
  thread_join(&tC);
}
```

![diagram](../img/cap5-ex8.jpg)