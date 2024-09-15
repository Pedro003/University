#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void conta(char* imagens[], int n){
    int p[2];
    if (pipe(p) == -1){
        perror("erro ao criar pipe");
    }
    int pid1 = fork();
    switch(pid1){
        case -1:
            perror("erro ao criar filho");
            break;
        case 0:
            close(p[0]);
            dup2(p[1],1);
            close(p[1]);
            defeitos(imagens, n, 3);
            exit(0);
        default:
            break; 
    }       
    int pid2 = fork();
    switch(pid2){
        case -1:
            perror("erro ao criar filho");
            break;
        case 0:
            close(p[1]);
            dup2(p[0],0);
            close(p[0]);
            execlp("wc","wc","-l",NULL);
            perror("erro ao fazer execlp");
            exit(1);
        default:
            break;
    }

    close(p[0]);
    close(p[1]);
    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);
}