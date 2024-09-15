#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
 
#define BUF_SIZE 1024

// está incompleto

int main(){

    int p[2];
    

    if(pipe(p) == -1){
        perror("Erro na criação do pipe\n");
        exit(EXIT_FAILURE);
    }

    int fork1 = fork();
    if(fork1 == 0){
        close(p[0]);

        dup2(p[1], 1);
        close(p[1]);

        int exec_res = execlp("ls", "ls", "/etc", NULL);
        perror("execlp ls\n");
        exit(EXIT_FAILURE);
    }

    int fork2 = fork();
    if(fork2 == 0){
        close(p[1]);
        dup2(p[0], 0);
        close(p[0]);

        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc\n");
        exit(EXIT_FAILURE);
    }
    close(p[0]);
    close(p[1]);

    waitpid(fork1, NULL, 0);
    waitpid(fork2, NULL, 0);

    return 0;
}