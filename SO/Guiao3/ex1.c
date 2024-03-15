#include <stdio.h>


int main(int argc, char *argv[]){
    
    printf("ex1\n");

    int res = execl("/bin/ls", "ls", "-l", NULL);
    
    printf("res = %d\n", res);

    return 0;
}