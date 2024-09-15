#include "aux.h"

int main(int argc, char* argv[]){
    // criação do folder onde estaram alocados os ficheiros com o output das tasks pedidas por cliente + folder para onde seram criados todos os fifos
    if ((mkdir(argv[1], 0777) == -1 || mkdir("../FIFOS", 0777) == -1)) {
        perror("Error creating directory");
        return 1;
    }
    // criar fifos
    if (mkfifo("../FIFOS/lista", 0666) == -1 || mkfifo("../FIFOS/execs", 0666) == -1 || mkfifo("../FIFOS/servidor", 0666) == -1){
                perror("Error creating FIFO");
                return 1;
    }
    char *inicio = "Servidor á espera de Clientes...\n";
    write(1, inicio, strlen(inicio));
    int task = 0;
    int i;
    struct timeval tempo_final, duracao;
    ClientData data;
    ClientData array_executing[atoi(argv[2])];
    for(int j = 0; j < atoi(argv[2]); j++) array_executing[j].id = 0;
    Queue waitingQueue;
    initQueue(&waitingQueue); // inicia a fila


    // abrir pipe para ler e escrever (assim garante que o servidor permaneça aberto)
    int leituras = open("../FIFOS/servidor", O_RDONLY);
    int escritas = open("../FIFOS/servidor", O_WRONLY);
    while (read(leituras, &data, sizeof(data)) > 0) {
        // serve para sair do ciclo while e terminar o FIFO(servidor) após todas as tarefas sairem da fila de espera para executar
        if(strcmp(data.command, "end") == 0){ 
            if (isEmptyQueue(&waitingQueue)) close(escritas);
            else write(escritas, &data, sizeof(data));
        }

        if(strcmp(data.command, "status") == 0){
            if(fork() == 0){
                int lista , execs;
                Queue copia = waitingQueue;
                if((lista = open("../FIFOS/lista", O_WRONLY, 0777)) == -1){
                    perror("erro ao abrir FIFO\n");
                    return 1;
                }
                while(!isEmptyQueue(&copia)) { // iterar sobre cada elemento da fila e escrever no FIFO
                    ClientData removedData = dequeue(&copia);
                    if (write(lista, &removedData, sizeof(removedData)) == -1) { 
                        perror("erro ao escrever no fifo\n");
                        close(lista);
                    return 1;
                    }
                }
                close(lista);
                if((execs = open("../FIFOS/execs", O_WRONLY, 0777)) == -1){
                    perror("erro ao abrir FIFO\n");
                    return 1;
                }
                for(int j = 0; j < atoi(argv[2]); j++) {
                    if(array_executing[j].id != 0) {
                        if (write(execs, &array_executing[j], sizeof(array_executing[j])) == -1) {
                            perror("erro ao escrever no fifo\n");
                            close(execs);
                            return 1;
                        }
                    }
                }
                close(execs);
                _exit(EXIT_SUCCESS);
            }
        }
        

        // zona das tasks
        if(strcmp(data.command, "execute") == 0){
            // se a task que voltou a ser escrita para o servidor vier com valor de status == 1 sei que terminou e tenho de a remover do array_executing
            if(data.status == 3){
                for (int i = 0; i < atoi(argv[2]); i++)
                {
                    if(array_executing[i].id == data.id){
                        array_executing[i].id = 0;
                        // retira uma task de scheduled e escreve de volta para o servidor
                        if(!isEmptyQueue(&waitingQueue)){
                            ClientData removedData = dequeue(&waitingQueue); // remove um elemento da fila e armazena em removedData
                            write(escritas, &removedData, sizeof(ClientData));
                        }
                    }
                }
            }
            else{
                if(data.status == 0){
                    task++;
                    data.id = task;
                    int fd1 = open(data.fifo, O_WRONLY); // abrir pipe para escrita
                    sprintf(data.mensagem, "Task %d received\n", data.id);
                    write(fd1, &data, sizeof(ClientData)); // escrever para o pipe
                    close(fd1);
                }
                for (i = 0; i < atoi(argv[2]); i++) {
                    if (array_executing[i].id == 0) { // ver se o array que aloca as tarefas q estao em execução está cheio
                        array_executing[i] = data; // escreve na primeira posição livre do array_executing
                        array_executing[i].status = 1; // passa para status 1(executing)

                        int fork_res = fork();
                        // processo filho
                        if (fork_res == 0){
                            int tarefas_terminadas = open("Output/Tarefas_terminadas.txt", O_CREAT | O_WRONLY | O_APPEND, 0777);
                            //sleep(20);
                            char tarefa[40]; // criação da variavel tarefa que vai alocar o respetivo file onde será printado o output da tarefa pretendida
                            sprintf(tarefa, "%s/Task%d.txt", argv[1], data.id);
                            if(strcmp(data.tipo, "-u") == 0){ // ! executar as tasks -u 
                                mysystem_U(data.prog_args, tarefa); // funcao criada para executar o programa + args e redireciona o output para um ficheiro
                                data.status = 3; // status == 3(Completed)
                                write(escritas, &data, sizeof(ClientData)); // escreve de volta para o servidor agora com status == 1(Completed)
                            }
                            if(strcmp(data.tipo, "-p") == 0){ // ! executar as tasks -p
                                mysystem_P(data.prog_args, tarefa);
                                data.status = 3; // status == 3(Completed)
                                write(escritas, &data, sizeof(ClientData)); // escreve de volta para o servidor agora com status == 1(Completed)
                            }
                            gettimeofday(&tempo_final, NULL);
                            timersub(&tempo_final, &data.tempo_inicial, &duracao); // alocação do tempo de execução da tarefa na variavel "duracao"
                            long tempo = duracao.tv_sec * 1000 + duracao.tv_usec / 1000;
                            // calcular o tamanho da string que representa o formato de cada tarefa terminada
                            int length = snprintf(NULL, 0, "%d %s %ld ms\n", data.id, data.prog_args, tempo);
                            char terminada[length + 1];
                            sprintf(terminada, "%d %s %ld ms\n", data.id, data.prog_args, tempo);
                            write(tarefas_terminadas, &terminada, strlen(terminada));
                            close(tarefas_terminadas);
                            _exit(EXIT_SUCCESS);
                        }
                        // processo pai
                        else if(fork_res > 0){
                            break;
                        }
                        // caso o fork() de erro
                        else{
                            perror("Erro a dar fork\n");
                            _exit(EXIT_FAILURE);
                        }
                    }
                }
                // significa que o array_executing está cheio e vai ter de deixar as tarefas numa lista de espera
                if(i == atoi(argv[2])){
                    // printf("incrementa task\n");
                    data.status = 2;
                    // incrementa uma task na fila de espera
                    enqueue(&waitingQueue, data);
                }
            }
        }
    }// sai do ciclo while que lê para o servidor
    close(leituras);
    unlink("../FIFOS/lista");
    unlink("../FIFOS/execs");          
    unlink("../FIFOS/servidor");
    return 0;
}