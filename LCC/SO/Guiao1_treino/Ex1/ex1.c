#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define processa 100

int mycat(){
    int leitura;
    int escrita;
    char buffer [processa];

    while((leitura = read(0, buffer, processa)) > 0){
        printf("li %d bytes\n", leitura);

        escrita = write(1, buffer, leitura);
        printf("\nescrevi %d bytes\n", escrita);

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