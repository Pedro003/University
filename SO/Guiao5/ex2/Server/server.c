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

	//TODO
	int res = mkfifo("servidor", 0666);
    
    if (res == -1){
        perror("erro na criação do fifo\n");
    }

	printf("vou ler do fifo");

    int fd = open ("servidor", O_RDONLY);
    if (fd == -1){
        perror("erro ao abrir fifo para leitura");
        return 1;
    }

    char buf[SIZE];
    int bytes_read;
    printf("vou ler do fifo\n");
    while ((bytes_read = read(fd, buf, SIZE)) > 0){
        // escrever para o fifo
        write(1, buf, bytes_read);

		// pela stora 

		//open FIFO client for writing response 
    }

    close(fd);	

	unlink("servidor");

	
	return 0;
}
