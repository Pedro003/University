#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"

int main (int argc, char * argv[]){

    if (argc < 2) {
        printf("Missing argument.\n");
        _exit(1);
    }

    // Create FIFO for client-server communication
    char buf[20];
    sprintf(buf, "../Server/%d", getpid());
    int res = mkfifo(buf, 0666);
    if (res == -1) {
        perror("erro na criação do fifo do cliente\n");
        return 1;
    }

    // Open FIFO for writing
    int fd = open("../Server/servidor", O_WRONLY);
    if(fd == -1){
        perror("erro ao abrir o fifo");
        return 1;
    }

    // Write to FIFO
    Msg m;
    m.needle = atoi(argv[1]);
    m.occurrences = 0;
    m.pid = getpid();
    write(fd, &m, sizeof(m));
    close(fd);

    // Open FIFO for reading response from server
    if ((fd = open(buf, O_RDONLY)) == -1){
        perror("erro a abrir fifoClient");
        return -1;
    }

    // Read response from server
    read(fd, &m, sizeof(m));
    close(fd);
    // Remove FIFO
    unlink(buf);

    // Display result
    printf("Ocr: %d\n",m.occurrences);
    return 0;
}


