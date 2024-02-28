#include "person.h"

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 3) {
        perror("Número de argumentos incorreto");
        return 1;
    }
    if (strcmp(argv[1],"-i") == 0) {
        printf("comando -i executado\n");
        add_person(argv[2], atoi(argv[3]));
    }
    else if (strcmp(argv[1],"-l") == 0){
        printf("comando -l executado\n");
        list_person(atoi(argv[2]));
    }
    else if (argv[1] == "-u"){

    }
    else{
        printf("argumento %s não suportado", argv[1]);
    }

    return 0;
}












