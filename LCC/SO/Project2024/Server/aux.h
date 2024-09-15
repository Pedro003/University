#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include "../Client/DataStruct.h"

#define limite 300 // tamanho maximo de args

#define MAX_QUEUE_SIZE 100 // tamanho maximo da fila

typedef struct {
    ClientData data[MAX_QUEUE_SIZE];
    int front, rear; // indices da frente e do fim da fila
    int count; // contagem de elementos na fila
} Queue;

void initQueue(Queue *queue);
int isEmptyQueue(Queue *queue);
int isFullQueue(Queue *queue);
void enqueue(Queue *queue, ClientData newData);
ClientData dequeue(Queue *queue);




int mysystem_U(const char *command, char *tarefa);
int mysystem_P(const char *command, char *tarefa);

