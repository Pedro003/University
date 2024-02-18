#include "Estrutura.h"


int armazena(struct Pessoa *pessoas, char *opcao, char *nome, char *idade) {
    ssize_t bytes_escritos;
    char buffer[BUFFER_SIZE];
    int i = 0;

    int fa = open("armazenamento.txt", O_WRONLY | O_CREAT);
    if (fa == -1) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    if (strcmp(opcao, "-i") == 0) {
        while (pessoas[i].idade != 0) {
            strcpy(pessoas[i].nome, nome);
            pessoas[i].idade = atoi(idade);
            i++;
        }

        bytes_escritos = write(fa, pessoas, sizeof(struct Pessoa) * i);
        if (bytes_escritos == -1) {
            perror("Erro ao escrever no arquivo");
            close(fa);
            return -1;
        }
    }

    close(fa);
    return 0;
}