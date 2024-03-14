#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h>
#include <stdio.h>


int main(){
    int res;
    int i;
    for(i= 0; i<10; i++){
        res = fork();

        if(res == 0){
            printf("Pid FILHO: %d\nPpid FILHO: %d, i:%d\n", getpid(),getppid(),i);
            sleep(1);
            //filho - imprimir pid, ppid e o i
            _exit(i);
        }
        else{
            int status;
            int terminated_pid = wait(&status);
            // imprimir o valor de retorno do filho
            if (WIFEXITED(status)){
                printf("PAI: terminei o filho %d com %d\n", terminated_pid, WEXITSTATUS(status));
            }
        }
    }
}
