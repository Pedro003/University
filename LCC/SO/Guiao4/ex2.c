#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


int PaiParaFilho(){

    int fds[2];

    if(pipe(fds) == -1){
        perror("erro no pipe");
        return -1;
    }

    int fork_res = fork();
    switch(fork_res){
        case 1: {
            perror("erro no fork");
            return -1;
            break;
        }
        case 0: {
            //filho
            close(fds[0]);
            int value = 0;
            for ( int i = 0; i<=100000; i++){
                value = i;
                printf("FILHO | vou escrever o inteiro %d\n", value);
                int bytes_written = write(fds[1], &value, sizeof(int));
                printf("FILHO | escrevi %d bytes\n", bytes_written);
            }
            close(fds[1]);
            _exit(0);
        }

        default: {
            //pai
            close(fds[1]);
            int value_read;
            int bytes_read = 1;
            sleep(5);
            while(bytes_read > 0){ // quando bytes_read = 0 significa end of file o que quer dizer que ja nao existem mais bytes para ler fazendo assim o processo pai sair do ciclo e fechar o descritor de leitura
                printf("PAI | vou ler um inteiro\n");
                bytes_read = read(fds[0], &value_read, sizeof(int));
                printf("PAI | li %d bytes com o valor %d\n", bytes_read, value_read);
            }
            close(fds[0]);
            

            int status;
            int wait_res = wait(&status);
            if(WIFEXITED(status)){
                printf("PAI | filho %d terminou com valor %d\n", wait_res, WEXITSTATUS(status));
                return WEXITSTATUS(status);
            }
            else{
                printf("PAI | filho %d nao terminou corretamente", wait_res);
                return -1;
            }
        }
    }
}

int main(int argc, char*argv[]){
    return PaiParaFilho();
}