#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUF_SIZE 1024


int main(){

    printf("vou escrever para o fifo\n");

    int fd = open("fifo", O_WRONLY);
    if(fd == -1){
        perror("erro ao abrir o fifo");
        return 1;
    }

    char buf[BUF_SIZE];
    int bytes_read;
    // ler do fifo
    printf("vou ler do stdin\n");
    while ((bytes_read = read(0, buf, BUF_SIZE)) > 0){
        // escrever para o fifo
        write(fd, buf, bytes_read);
    }

    close(fd);
    return 0;
}