#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define processa 1

int mycat(){
    ssize_t leitura;
    ssize_t escrita;
    char buffer [processa];

    while((leitura = read(0, buffer, processa)) > 0){
        printf("li %ld bytes\n", leitura);

        escrita = write(1, buffer, processa);
        printf("\nescrevi %ld bytes\n", escrita);

        if (leitura != escrita){
            return 1;
        }
    }
    printf("terminei...");

    return 0;
}

int main(){
    return mycat();
}