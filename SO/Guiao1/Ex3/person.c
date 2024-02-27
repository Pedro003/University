#include "person.h"


int add_person(char *name, int age){

    int fd = open(FILENAME, O_WRONLY | O_CREAT, 0600);
    if(fd == -1){
        perror("erro ao abrir ficheiro");
        return -1;
    }

    Person p;
    p.age = age;
    strcpy(p.name,name);


    ssize_t bytes_escritos = write (fd, &p, sizeof(Person));
    if (bytes_escritos < 0){
        perror("erro ao compilar");
        return -1;
    }

    printf("Adicionei a pessoa: nome:%s, idade:%d\n", p.name, p.age);

    close(fd);
    return 0;
}





int list_person(int N){
    ssize_t bytes_lidos;
    ssize_t bytes_escritos;
    int fd = open(FILENAME, O_RDONLY);
    if (fd<0){
        perror("erro ao abrir ficheiro");
        return -1;
    }
    Person p;
    int i = 0;

    while((bytes_lidos = read(fd, &p ,sizeof(Person))) > 0){
        i++;
    
    if(i<N){
        bytes_escritos = write (1, &p, sizeof(Person));
    }
    else{
        return i;
    }
    }

    close(fd);
    return 0;
}

int update_age(char *name, int age){
    return 0;
}