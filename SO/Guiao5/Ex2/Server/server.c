#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "../Cliente/defs.h"
#include "vector.h"



//FIFO criado pelo servidor
//Cliente pode receber um sigpipe (concorrência!)

int main (int argc, char * argv[]){
    
    init_vector();
    print_vector();

    // Create FIFO
    int res = mkfifo("servidor", 0666);
    if (res == -1) {
        perror("erro na criação do fifo\n");
        return 1;
    }


    // Open FIFO for reading
    int fd = open("servidor", O_RDONLY);
    if (fd == -1){
        perror("erro ao abrir fifo para leitura");
        return 1;
    }

    // Open FIFO for writing back to the client
    int fd_write = open("servidor", O_WRONLY); 
    if(fd_write == -1){
        perror("erro ao abrir fifo para escrita");
        return 1;
    }

    Msg m;
    int pid;

    while (read(fd, &m, sizeof(m)) > 0){
        pid = fork();
        if (pid == -1){
            perror("erro ao criar processo");
            return -1;
        }
        if (pid == 0){
            //processo filho
            m.occurrences = count_needle(m.needle);
            char buf[20];
            sprintf(buf,"%d", m.pid);
            int sc = open(buf, O_WRONLY);
            write(sc, &m, sizeof(m));
            close(sc);
            _exit(0);
        }
    }

    // Close file descriptors
    close(fd); 
    close(fd_write); // Close the FIFO used for writing back to the client

    // Remove FIFO
    unlink("servidor");

    return 0;
}
