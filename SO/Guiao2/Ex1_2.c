#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){

    int res = fork();

    if(res == 0){
        //filho
        printf("FILHO: PID: %d, PPID: %d\n", getpid(), getppid());
        sleep(4);
        printf("FILHO %d terminei\n", getpid());
        _exit(30); // entre 0 e 254 permite
    }
    else{
        //pai
        printf("PAI: PID: %d, PPID: %d, pid filho = %d\n", getpid(), getppid(), res);
        sleep(5);

        printf("PAI: vou esperar pelo filho\n");
        int status;
        int pid_terminou = wait(&status);
        printf("PAI: terminou o filho %d\n", pid_terminou);

        if (WIFEXITED(status)){
            printf("PAI: terminei o filho %d com %d\n", pid_terminou, WEXITSTATUS(status));
        }
    }

    printf("PAI terminou\n");

return 10;
}