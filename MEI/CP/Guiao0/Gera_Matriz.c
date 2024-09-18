#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000

int** gera_matriz() {
    int** matriz = (int**)malloc(N * sizeof(int*));
    if (matriz == NULL) {
        return NULL;
    }
    for (int i = 0; i < N; i++) {
        matriz[i] = (int*)malloc(N * sizeof(int));
        
        if (matriz[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }

        for (int j = 0; j < N; j++) {
            matriz[i][j] = rand() % 100;
        }
    }

    return matriz;
}

int** mult_matrizes(int **matriz1, int **matriz2) {
    int** matriz = (int**)malloc(N * sizeof(int*));
    if (matriz == NULL) {
        return NULL;
    }
    for (int i = 0; i < N; i++) {
        matriz[i] = (int*)malloc(N * sizeof(int));
        
        if (matriz[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }

        for (int j = 0; j < N; j++) {
            matriz[i][j] = 0;
            for (int k = 0; k < N; k++) {
                matriz[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return matriz;
}

int main() {
    int** matriz1 = gera_matriz();
    int** matriz2 = gera_matriz();

    if (matriz1 == NULL || matriz2 == NULL) {
        printf("Erro ao alocar a matriz.\n");
        return 1;
    }

    int** matriz3 = mult_matrizes(matriz1, matriz2);

    if (matriz3 == NULL) {
        printf("Erro ao alocar a matriz resultante.\n");
        return 1;
    }

    printf("O valor retirado da matriz foi: %d\n", matriz3[rand() % N][rand() % N]);

    for (int i = 0; i < N; i++) {
        free(matriz1[i]);
        free(matriz2[i]);
        free(matriz3[i]);
    }
    free(matriz1);
    free(matriz2);
    free(matriz3);

    return 0;
}
