/*5. Pretende-se determinar a existˆencia de um determinado n  ́umero inteiro nas linhas de numa matriz de
n  ́umeros inteiros, em que o n  ́umero de colunas  ́e muito maior do que o n  ́umero de linhas. Implemente,
utilizando processos um programa que determine a existˆencia de um determinado n  ́umero, recebido como
argumento, numa matriz gerada aleatoriamente.*/

#include "matrix.h"


int main(int argc, char *argv[]){
    // generate random matrix
    int **matrix = createMatrix();

    // print matrix
    printMatrix(matrix);

    // TO DO
    //valueExists(matrix, atoi(argv[1]));
    linesWithValue(matrix, atoi(argv[1]));

    // free matrix
    for (int i = 0; i < ROWS; i++) {
        free(matrix[i]);
    }
    free(matrix);



    return 0;
}