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

    //popular a struct
    

	//TODO
	printf("vou escrever para o fifo\n");

    int fd = open("../Server/servidor", O_WRONLY);
    if(fd == -1){
        perror("erro ao abrir o fifo");
        return 1;
    }

    char buf[SIZE];
    int bytes_read;
    // ler do fifo
    printf("vou ler do stdin\n");
    while ((bytes_read = read(0, buf, SIZE)) > 0){
        // escrever para o fifo
        write(fd, buf, bytes_read);
    }

    close(fd);
	
	return 0;
}

