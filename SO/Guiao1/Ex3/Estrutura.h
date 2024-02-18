#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1000

struct Pessoa
{
    char nome[100];
    int idade;
};


int armazena(struct Pessoa *pessoas, char *opcao, char *nome, char *idade);