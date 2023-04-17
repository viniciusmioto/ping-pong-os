#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    // bloco A: CRIAÇÃO DO PRIMEIRO FILHO
    printf("#A# Sou %5d, filho de %5d\n", getpid(), getppid());
    int x = 0;
    fork(); 
    x++;
    sleep(5);

    // BLOCO B: ESPERA DO PRIMEIRO FILHO
    printf("#B# Sou %5d, filho de %5d | X = %d\n", getpid(), getppid(), x);
    wait(0);   

    // BLOCO C: CRIAÇÃO DO SEGUNDO FILHO
    printf("#C# Sou %5d, filho de %5d | X = %d\n", getpid(), getppid(), x);
    fork();

    // BLOCO D: ESPERA DO SEGUNDO FILHO
    printf("#D# Sou %5d, filho de %5d | X = %d\n", getpid(), getppid(), x);
    wait(0);
    
    // BLOCO E: FINAL
    sleep(5);
    x++;
    printf("#E# Sou %5d, filho de %5d | X = %d\n", getpid(), getppid(), x);
}