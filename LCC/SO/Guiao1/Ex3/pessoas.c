#include <stdio.h>
#include <string.h>
#include "person.h"


int main(int argc, char* argv[]){

    if ( argc < 3 )
    {
        printf("Usage:\n");
        printf("Add new person: ./pessoas -i [name] [age]\n");
        printf("List N persons: ./pessoas -l [N]\n");
        printf("Change person age: ./pessoas -u [name] [age]\n");
        printf("Change person age (v2): ./pessoas -o [position] [age]\n");
        return 1;
    }

    if ( strcmp(argv[1],"-i") == 0 )
    {
        int pos = add_person(argv[2], atoi(argv[3]));
        char buffer[15];
        int size = sprintf(buffer,"registo %d\n", pos);
        write(1, buffer, size);
    }

    if ( strcmp(argv[1],"-l") == 0 )
    {
        list_person(atoi(argv[2]));
    }

    if ( strcmp(argv[1],"-u") == 0 )
    {
        update_age(argv[2], atoi(argv[3]));
    }
    // ! nao percebi o que é suposto fazer nesta
    //if ( strcmp(argv[1],"-o") == 0 )
    //{
    //    update_age_V2(atoi(argv[2]), atoi(argv[3]));
    //}

    return 0;
}
