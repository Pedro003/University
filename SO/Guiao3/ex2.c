#include <stdio.h>
#include <sys/wait.h>

int main(){

    printf("PAI | ex2\n");

    int res = 0, status, filho;

    char *command[3] = {"ls", "-l", NULL};

    pid_t pid = fork();

    switch(pid){
        case -1:
            perror("ERRO no fork");
            res = -1;
            break;
        case 0:
            // estamos no processo filho
            //res = execlp("ls", "ls", "-l", NULL);
            res = execvp(command[0], command); //ou res = execvp("ls", args);
            perror("erro no exec");
            _exit(res);
            break;    
        default:
            // estamos no processo pai
            filho = wait(&status);
            if(WIFEXITED(status)){
                printf("Filho %d devolveu %d\n", filho, WEXITSTATUS(status));
            }

            break;    
    }


    return res;
}