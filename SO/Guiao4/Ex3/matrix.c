#include "matrix.h"


int **createMatrix() {

    // seed random numbers
    srand(time(NULL));

    // Allocate and populate matrix with random numbers.
    printf("Generating numbers from 0 to %d...", MAX_RAND);
    int **matrix = (int **) malloc(sizeof(int*) * ROWS);
    for (int i = 0; i < ROWS; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * COLUMNS);
        for (int j = 0; j < COLUMNS; j++) {
            matrix[i][j] = rand() % MAX_RAND;
        }
    }
    printf("Done.\n");

    return matrix;
}

void printMatrix(int **matrix) {

    for (int i = 0; i < ROWS; i++) {
        printf("%2d | ", i);
        for (int j = 0; j < COLUMNS; j++) {
            printf("%7d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void lookupNumber(int** matrix, int value, int* vector){

    //TODO
    //Hint - use the Minfo struct from matrix.h!
    int p[2];

    if(pipe(p) == -1){
        perror("erro no pipe");
        return -1;
    }

    for(int i=0; i<ROWS; i++){
        int fork_res = fork();
        switch(fork_res){
            case -1: {
                perror("erro no fork");
                return -1;
                break;
            }
            case 0: {
                close(p[0]);
                Minfo n;
                n.line_nr = i;
                n.ocur_nr = 0;
                for(int j=0; j<COLUMNS; j++){
                    if(matrix[i][j] == value){
                        n.ocur_nr++;
                    }
                }
                int bytes_written = write(p[1], &n, sizeof(Minfo));
                close(p[1]);
                _exit(0);
            }
            default: {
                break;
            }

        }
    }
    // ler

    close(p[1]);
    Minfo ni;
    while(read(p[0], &ni, sizeof(Minfo)) > 0) {
        vector[ni.line_nr] = ni.ocur_nr;
    }

    close(p[0]);

    // esperar (falta ver aqui)!!
    int status; // n tenho certeza se é aqui!!
    for(int i = 0; i < ROWS; i++){
        int wait_res = wait(&status);
        if(WIFEEXITED(status)){
            printf("PAI | filho %d terminou com valor %d\n", wait_res, WEXITSTATUS(status));
        }
        else{
            printf("PAI | filho %d nao terminou corretamente", wait_res);
        }
    }

}