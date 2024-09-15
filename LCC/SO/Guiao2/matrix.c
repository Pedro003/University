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
                    _exit(i);
                }
            }
            _exit(-1);
        }
    }
    int res = 0;
    pid_t pid;
    for(int i=0; i<ROWS; i++){
        int status;
        pid = wait(&status);
        if(WIFEXITED(status)){
            if (WEXITSTATUS(status) < 255){
                printf("Pai: %d, esperei pelo processo Filho:%d - %d\n", getpid(), WEXITSTATUS(status), pid);
                res = 1;
            }
            else{
                printf("Process %d, exited. nothing found (linha %d)\n", pid ,WEXITSTATUS(status));
            }
        }
        else{
            printf("Process %d, exited. something went wrong\n", pid);
        }
    }    

    return res;
}


// ex.6
int linesWithValue(int **matrix, int value) {
    pid_t pid;
    pid_t pids[ROWS];
    int status;


    for(int i=0; i<ROWS; i++){
        pid = fork();
       if (pid == 0) {
            //printf("[pid %d] row: %d\n", getpid(),i);
            for(int j=0; j<COLUMNS; j++){
                if(matrix[i][j] == value){
                    _exit(i);
                }
            }
            _exit(-1);
        }
        else pids[i] = pid;
    }

    for(int i=0; i<ROWS; i++){
        int status;
        pid = waitpid(pids[i], &status, 0);
        if(WIFEXITED(status)){
            if (WEXITSTATUS(status) < 255){
                printf("Filho: %d - encontrou numero:%d - linha: %d\n", pid, value, WEXITSTATUS(status));
            }
        }
        else{
            printf("Process %d, exited. something went wrong\n", pid);
        }
    }    

    return 0;
}