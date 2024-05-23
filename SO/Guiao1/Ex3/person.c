#include "person.h"


int add_person(char *name, int age){

    int fd = open(FILENAME, O_WRONLY | O_CREAT | O_APPEND, 0600);
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

    int pos = lseek(fd, -sizeof(Person), SEEK_CUR);

    printf("Adicionei a pessoa: nome:%s, idade:%d\n", p.name, p.age);

    close(fd);

    return pos/sizeof(Person);
}





int list_person(int N){
    ssize_t bytes_lidos;
    int fd = open(FILENAME, O_RDONLY);
    if (fd<0){
        perror("erro ao abrir ficheiro");
        return -1;
    }
    Person p;
    int i = 0;

    while((bytes_lidos = read(fd, &p ,sizeof(Person))) > 0){
        i++;
    
        if(i<=N){
            char buffer[200];
            int size = sprintf(buffer,"pessoa:%s age:%d\n",p.name, p.age);
            write(1,buffer,size);
        }
        else{
            return i;
        }
    }

    close(fd);
    return 0;
}

int update_age(char *nome, int age){
    int fd = open(FILENAME, O_RDWR);
    ssize_t bytes_lidos;
    Person p;

    while((bytes_lidos = read(fd, &p, sizeof(Person))) > 0){
        if(strcmp(p.name,nome) == 0){
            lseek(fd, -bytes_lidos, SEEK_CUR);
            p.age = age;
            write(fd, &p, sizeof(Person));
            close(fd);
            printf("idade de %s atualizada com sucesso!\n",p.name);
            return 1;
        }
    }
    close(fd);
    printf("Pessoa não existe\n");
    return 0;
}


//int update_age_v2(int pos, int age){
//    int fd = open(FILENAME, O_WRONLY);
//    lseek(fd, sizeof(Person)*pos, SEEK_CUR);
//    lseek()
//    write(fd, )
//    Person p;
//
//    return 0;
//}



