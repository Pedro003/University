



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
            close(fds[1]);
            int value_read;
            printf("FILHO | vou ler um inteiro\n");
            int bytes_read = read(fds[0], &value_read, sizeof(int));
            printf("FILHO | li %d bytes com o valor %d\n", bytes_read, value_read);
            close(fds[0]);
            printf("FILHO | terminei!\n");
            _exit(0);
        }

        default: {
            //pai
            close(fds[0]);
            int value = 300;
            // teste alínea (a)-- sleep(5);
            printf("PAI | vou escrever o inteiro %d\n", value);
            int bytes_written = write(fds[1], &value, sizeof(int));
            printf("PAI | escrevi %d bytes\n", bytes_written);
            close(fds[1]);

            int status;
            int wait_res = wait(&status);
            if(WIFEEXITED(status)){
                printf("PAI | filho %d terminou com valor %d\n", wait_res, WEXITSTATUS(status));
            }
            else{
                printf("PAI | filho %d nao terminou corretamente", wait_res);
            }
        }
    }
}