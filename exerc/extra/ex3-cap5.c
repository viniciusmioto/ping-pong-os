#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t pid[10];
    int i;
    printf("==> Sou %5d, filho de %5d\n", getpid(), getppid());
    
    int N = atoi(argv[argc - 2]);

    for (i = 0; i < N; i++) {
        printf("gerando filho %d\n", i);
        pid[i] = fork();    
    }

    printf("pid ATUAL: %d, pid[0] = %d, pid[N - 1] = %d\n", getpid(), pid[0], pid[N - 1]);
    if (pid[0] != 0 && pid[N - 1] != 0) {
        printf("gerando filho %d\n", N);
        pid[N] = fork();
    }

    printf("-> Sou %5d, filho de %5d\n", getpid(), getppid());
}