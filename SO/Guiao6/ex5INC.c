#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
 
#define BUF_SIZE 1024

// está incompleto

int main(){

    int p[2];
    int bytes_read;
    char buf[BUF_SIZE];

    pipe(p);

    int fork_res = fork();
    if(fork_res == 0){
        close(p[1]);

        dup2(p[0], 0);
        close(p[0]);

        int exec_res = execlp("wc", "wc", "-l", NULL);
        _exit(exec_res);
    }

    close(p[0]);
    dup2(p[1], 1);
    while((bytes_read = read(0, buf, BUF_SIZE)) > 0){
        write(1, buf, bytes_read);
    }
    close(p[1]);

    wait(NULL);

    return 0;
}