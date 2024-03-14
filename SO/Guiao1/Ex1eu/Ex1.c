#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <stdio.h>
#define BUFFER_SIZE 6

int mycat(){
    ssize_t bytes_lidos;
    ssize_t bytes_escritos;
    char buffer [BUFFER_SIZE];

    //ler do stdin
    while((bytes_lidos = read(0, buffer, BUFFER_SIZE)) > 0){
    printf("li %ld bytes\n", bytes_lidos);
    

    //escrever para o stdout
    bytes_escritos = write(1, buffer, BUFFER_SIZE);
    printf("escrevi %ld bytes\n", bytes_escritos);


    // validar o que lemos foi o que escrevemos
    if (bytes_lidos != bytes_escritos){
        return 1;
    }
    }
    printf("terminei");

    return 0;
}



int main(){
    return mycat();
}



