#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h>
#include <stdio.h>


int main(){
    int res;
    int i;
    for(i= 0; i<10; i++){
        res = fork();
        if(res == 0){
            printf("Pid FILHO: %d, Ppid FILHO: %d, i:%d\n", getpid(),getppid(),i);
            break;
        }
    } 

    if(res == 0){
        _exit(i);
    }
    
    for(i = 0; i<10; i++){
        int status;
        int terminated_pid = wait(&status);
        // imprimir o valor de retorno do filho
        if (WIFEXITED(status)){
            printf("PAI: terminei o filho %d com %d\n", terminated_pid, WEXITSTATUS(status));
        }
    }
}