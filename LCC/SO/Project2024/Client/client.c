#include "comandos.h"

int main(int argc, char * argv[]){

    if (strcmp(argv[1], "execute") == 0 && argc == 5) {
        comando_execute(argv[2], argv[3], argv[4]);
    }
    else if (strcmp(argv[1], "status") == 0 && argc == 2) {
        comando_status(); 
    }   
    else if (strcmp(argv[1], "end") == 0 && argc == 2){
        char*end ="O servidor vai encerrar\n";
        write(STDOUT_FILENO, end, strlen(end));
        comando_end();
    }
    else {
        perror("ERROR , numero de argumentos inválido");
        return 1;
    }
    return 0;
}