typedef struct {
    int id; // numero da task
    int status; // se == 0 acabou de chegar uma task nova ao servidor se == 1 está executing se == 2 entrou modo scheduled e se == 3 completed   
    char command[10]; // nome do comando
    int time;
    char tipo[3]; // -u ou -p
    char prog_args[300]; // argumentos do programa
    char mensagem[40]; // mensagem para o cliente
    char fifo[40]; // nome do fifo do cliente
    struct timeval tempo_inicial; // tempo inicial de execucao da task
} ClientData;
