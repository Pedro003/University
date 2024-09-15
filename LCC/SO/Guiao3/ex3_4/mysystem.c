#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int mysystem(const char *command) {
    int status;
    pid_t pid;

    // Tokenize the command string into arguments
    char *exec_args[20];
    char *string, *cmd, *tofree;
    int i = 0;
    tofree = cmd = strdup(command);
    while ((string = strsep(&cmd, " ")) != NULL && i < 19) {
        exec_args[i++] = string;
    }
    exec_args[i] = NULL; // NULL-terminate the argument list

    pid = fork();

    if (pid < 0) { 
        perror("Fork failed");
        free(tofree); 
        return -1;
    } else if (pid == 0) { 
        execvp(exec_args[0], exec_args);
        perror("Execvp failed");
        free(tofree);
        exit(EXIT_FAILURE);
    } else { 
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            free(tofree);
            return WEXITSTATUS(status);
        }
    }

    free(tofree); 
    return -1;
}