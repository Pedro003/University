#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFFER_SIZE 10

int mycp(char *ficheiro_origem, char *ficheiro_destino){
    ssize_t ler;
    ssize_t escrever;
    char buffer[BUFFER_SIZE];

    int fl = open(ficheiro_origem, O_RDONLY);
    
    if(fl == -1){
        perror("ERRO ao abrir o ficheiro de leitura");
        return 1;
    }

    else{
        int fe = open(ficheiro_destino, O_CREAT | O_WRONLY, 0600);
        if (fe == -1){
            perror("ERRO ao abrir o ficheiro de escrita");
            return 1;
        }
        else{
            while((ler = read(fl,buffer,BUFFER_SIZE)) > 0){
                escrever = write(fe,buffer,ler);
            }
            close(fl);
            close(fe);
        }
    }
   return 0; 
}


int main(int argc, char *argv[]){
    return mycp(argv[1], argv[2]);
}