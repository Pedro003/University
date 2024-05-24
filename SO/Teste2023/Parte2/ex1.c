#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void defeitos(char* imagens[], int n, int max) {
    int conta = 0;
    int pid;

    for (int i = 0; i < n; i++) {
        if (conta >= max) {
            // Esperar por um processo filho terminar para abrir espaço para um novo
            wait(NULL);
            conta--;
        }

        pid = fork();
        if (pid == -1) {
            perror("Erro ao dar fork");
            exit(1);  // Sair em caso de erro no fork
        } else if (pid == 0) {
            // Processo filho executa o detectAnom
            execlp("./detectAnom", "./detectAnom", imagens[i], NULL);
            // Se chegar aqui, ocorreu um erro no exec
            perror("Erro no exec");
            _exit(1);  // Usar _exit para terminar imediatamente o processo filho
        } else {
            // Processo pai incrementa o contador de processos em execução
            conta++;
        }
    }

    // Esperar todos os processos filhos restantes terminarem
    while (conta > 0) {
        wait(NULL);
        conta--;
    }
}
