#include "Estrutura.h"

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 3) {
        perror("Número de argumentos incorreto");
        return 1;
    }

    struct Pessoa pessoas[BUFFER_SIZE]; // Alocando espaço para armazenar pessoas
    if (argc == 3) {
        // Lógica para listar pessoas
    } else {
        if (armazena(pessoas, argv[1], argv[2], argv[3]) == -1) {
            return 1;
        }
    }

    return 0;
}












