#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>

int main(){

    printf("PIPE_BUF: %d\n", PIPE_BUF);

    int res = mkfifo("fifo", 0666);
    
    if (res == -1){
        perror("erro na criação do fifo\n");
    }

    return 0;
}