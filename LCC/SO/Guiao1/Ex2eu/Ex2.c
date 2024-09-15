#include <fcntl.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <stdio.h>
#define BUFFER_SIZE 10

int mycp(char *ficheiro_origem, char *ficheiro_destino) {
    ssize_t bytes_lidos;
    ssize_t bytes_escritos;
    char buffer [BUFFER_SIZE];

    int fl =  open (ficheiro_origem,O_RDONLY);

    if (fl == -1){perror("Falha na abertura");
    return 1;
    }

    else{
        int fe = open (ficheiro_destino,O_WRONLY | O_CREAT, 0600);
        if(fe == -1){perror("falha na abertura");
            return 1;
            }
        while((bytes_lidos = read(fl, buffer, BUFFER_SIZE)) > 0){
            
            bytes_escritos = write(fe, buffer, bytes_lidos);
            
        }
        close(fe);
    }
    close(fl);
    return 0;
}

int main(int argc, char *argv[]){

    if (argc != 3) {
        fprintf(stderr, "Uso: %s arquivo_origem arquivo_destino\n", argv[0]);
        return 1;
    }


    return mycp(argv[1], argv[2]);
}



