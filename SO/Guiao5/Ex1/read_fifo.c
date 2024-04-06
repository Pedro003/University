#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUF_SIZE 1024

int main(){

    printf("vou ler do fifo");

    int fd = open ("fifo", O_RDONLY);
    if (fd == -1){
        perror("erro ao abrir fifo para leitura");
        return 1;
    }

    char buf[BUF_SIZE];
    int bytes_read;
    printf("vou ler do fifo\n");
    while ((bytes_read = read(fd, buf, BUF_SIZE)) > 0){
        // escrever para o fifo
        write(1, buf, bytes_read);
    }

    close(fd);

    unlink("fifo");

    return 0;
}