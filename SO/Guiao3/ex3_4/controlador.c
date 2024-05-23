#include <stdio.h>
#include "mysystem.h"

void controller(int N, char** commands) {
	int fork_ret, status;
    int pids[N];

    for (int i = 0; i < N; i++){
        fork_ret = fork();
        switch (fork_ret){
            case -1:{
                /* codigo em caso de erro*/
                perror("fork falhou");
                break;
            }
            case 0:{
                int counter = 0;
                int res = 1;
                while (res > 0){
                    res = mysystem(commands[i]);
                    counter++;
                }
                _exit(counter);
                break;
            }
            default:{
                /*codigo do processo pai*/
                pids[i] = fork_ret;
                break;
            }
        }
    }

    for (int i = 0; i < N; i++){
        waitpid(pids[i], &status, 0);
        if(WIFEXITED(status)){
            printf("Command %s executed %d times \n", commands[i], WEXITSTATUS(status));
        }
        else{
            printf("Monitor for command %s was interrupted \n", commands[i]);
        }
    }
}

int main(int argc, char* argv[]) {

	char *commands[argc-1];
	int N = 0;
	for(int i=1; i < argc; i++){
		commands[N] = strdup(argv[i]);
		printf("command[%d] = %s\n", N, commands[N]);
		N++;
	}

	controller(N, commands);

	return 0;
}