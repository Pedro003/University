#include "matrix.h"


int **createMatrix() {

    // seed random numbers
    // srand(time(NULL));
    srand(1000);

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

// ex.5
int valueExists(int **matrix, int value) {

    for(int i=0; i<ROWS; i++){
       if (fork() == 0) {
            printf("[pid %d] row: %d\n", getpid(),i);
            for(int j=0; j<COLUMNS; j++){
                if(matrix[i][j] == value){
                    printf("Process %d, exited. found number at row %d\n", getpid(), i);
                    _exit(i);
                }
            }
            printf("Process %d, exited. nothing found (linha 255)\n", getpid());
            _exit(i);
        }
    }
    pid_t pid;
    for(int i=0; i<ROWS; i++){
        int status;
        pid = wait(&status);
        if(WIFEXITED(status)){
            printf("Pai: %d, esperei pelo processo Filho:%d - %d\n", getpid(), WEXITSTATUS(status), pid);
        }
    }    

    return 0;
}


// ex.6
void linesWithValue(int **matrix, int value) {

    // TO DOm
}