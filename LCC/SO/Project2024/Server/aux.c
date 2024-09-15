#include "aux.h"

// usar o data.prog_args como input da função mysystem
int mysystem_U(const char *command, char *tarefa) {
    int status;
    pid_t pid;

    // Tokenize the command string into arguments
    char *exec_args[limite];
    char *string, *cmd, *tofree;
    int i = 0;
    tofree = cmd = strdup(command);
    while ((string = strsep(&cmd, " ")) != NULL && i < limite-1) {
        exec_args[i++] = string;
    }
    exec_args[i] = NULL; // NULL-terminate the argument list

    pid = fork();

    if (pid < 0) { 
        perror("Fork failed");
        free(tofree); 
        return -1;
    } else if (pid == 0) { 
        int file = open(tarefa, O_CREAT | O_WRONLY, 0777);
        if(file == -1){
            perror("erro ao abrir ficheiro");
            return 1;
        }
        dup2(file, STDOUT_FILENO);
        close(file);
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

int mysystem_P(const char *command, char *tarefa) {
    
    // Tokenize the command string into arguments
    int progs = 100;
    char *exec_args[progs][limite];
    char *string, *cmd, *tofree;
    int i = 0;
    int j = 0;
    tofree = cmd = strdup(command);
    while ((string = strsep(&cmd, "|")) != NULL && i < progs) {
        while (*string == ' ' || *string == '\t') string++; // skip de espaços em branco
        j = 0;

        // strtok para dividir os argumentos
        char *token = strtok(string, " ");
        while (token != NULL && j < limite) {
            exec_args[i][j++] = token;
            token = strtok(NULL, " ");
        }
        exec_args[i][j] = NULL;
        i++;
    }
    exec_args[i][0] = NULL; // NULL-terminate the last argument list

    int status;
    // executa os comandos em pipelines
    int p[i - 1][2];
    for (int k = 0; k < i; k++) { 
        if (pipe(p[k]) == -1) {
            perror("pipe failed");
            free(tofree);
            return -1;
        }
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            free(tofree);
            return -1;
        }

        // Processo filho
        if (pid == 0) { 
            // Redirect input for commands except the first one
            if (k > 0) {
                if (dup2(p[k - 1][0], STDIN_FILENO) == -1) {
                    perror("dup2 failed");
                    _exit(EXIT_FAILURE);
                }
                close(p[k - 1][0]);
            }

            // Redireciona o output para os comandos exceto o ultimo 
            if (k < i - 1) {
                if (dup2(p[k][1], STDOUT_FILENO) == -1) {
                    perror("dup2 failed");
                    _exit(EXIT_FAILURE);
                }
                close(p[k][1]);
            } else {
                int file = open(tarefa, O_CREAT | O_WRONLY | O_APPEND, 0777);
                if (file == -1) {
                    perror("erro ao abrir ficheiro");
                    _exit(EXIT_FAILURE);
                }
                if (dup2(file, STDOUT_FILENO) == -1) {
                    perror("dup2 failed");
                    _exit(EXIT_FAILURE);
                }
                close(file);
            }

            // Executa o comando com os argumentos
            execvp(exec_args[k][0], exec_args[k]);
            _exit(EXIT_SUCCESS);
        }

        // processo pai
        close(p[k][1]);
        if (k > 0) {
            close(p[k - 1][0]);
        }
    }

    // esperar pelo ultimo processo filho
    if (wait(&status) == -1) {
        perror("wait failed");
        return -1;
    }

    if (WIFEXITED(status)) {
        free(tofree);
        return WEXITSTATUS(status);
    } else {
        return -1;
    }
}

// fila de espera para as tarefas scheduled

// funcao para iniciar a fila
void initQueue(Queue *queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
}

// funcao para verificar se a fila está vazia
int isEmptyQueue(Queue *queue) {
    return queue->count == 0;
}

// funcao para verificar se a fila está cheia
int isFullQueue(Queue *queue) {
    return queue->count == MAX_QUEUE_SIZE;
}

// funcao para adicionar um elemento a fila (enqueue)
void enqueue(Queue *queue, ClientData newData) {
    if (!isFullQueue(queue)) {
        queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
        queue->data[queue->rear] = newData;
        queue->count++;
    } else {
        printf("Erro: fila cheia.\n");
    }
}

// funcao para remover um elemento da fila (dequeue)
ClientData dequeue(Queue *queue) {
    ClientData emptyData = {0}; // valor padrao para indicar fila vazia
    if (!isEmptyQueue(queue)) {
        ClientData dequeuedData = queue->data[queue->front];
        queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
        queue->count--;
        return dequeuedData;
    } else {
        printf("Erro: fila vazia.\n");
        return emptyData;
    }
}
