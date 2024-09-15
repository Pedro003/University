#include "comandos.h"
#include "DataStruct.h"

int comando_execute(char*tempo, char*cmd, char*string){
    int fd;
    ClientData data;
    data.id = 0;
    data.status = 0;
    strcpy(data.command, "execute");
    if(isdigit(*tempo)){
        data.time = atoi(tempo);
        if(strcmp(cmd, "-u") == 0){
            strcpy(data.tipo, "-u");
        }
        else if(strcmp(cmd, "-p") == 0){
            strcpy(data.tipo, "-p");
        }
        else{
            perror("!!comando fornecido inexistente!!\n");
            return 1;
        }
        strcpy(data.prog_args, string);
        //caminho para a criação do fifo juntamente com o pid do cliente 
        char fifo[40];
        sprintf(fifo, "../FIFOS/fifo%d", getpid());
        strcpy(data.fifo, fifo);
        // fifo para obter a conecção exata entre o cliente que enviou informação para o servidor 
        if (mkfifo(fifo, 0666) == -1) {
            perror("Error creating FIFO");
            return 1;
        }
        // abrir fifo(servidor) para escrita
        if((fd = open("../FIFOS/servidor", O_WRONLY)) == -1){
            perror("erro ao abrir o fifo");
            return 1;
        };
        gettimeofday(&data.tempo_inicial, NULL);
        // escrever a struct para o pipe
        write(fd, &data, sizeof(ClientData));
        close(fd);
        // abrir pipe para ler
        if((fd = open(fifo, O_RDONLY)) == -1){
            perror("erro ao abrir o fifo");
            return 1; 
        };
        // lê a resposta do Server
        if (read(fd, &data, sizeof(ClientData)) == -1) {
            perror("Error reading from FIFO");
            close(fd);
            return 1;
        }
        write(1, &data.mensagem, strlen(data.mensagem));
        close(fd);
        unlink(fifo);
    }
    else{
        perror("!!argumento relativo ao tempo incorreto!!\n");
        return 1;
    }
    return 0;
}

int comando_end(){
    int fd;
    ClientData data;
    if((fd = open("../FIFOS/servidor", O_WRONLY)) == -1){
                perror("erro ao abrir o fifo");
                return 1;
    };
    strcpy(data.command, "end");
    // escrever a struct para o pipe
    write(fd, &data, sizeof(ClientData));
    close(fd);
    return 0;
}


int comando_status(){
    int fd , lista, execs;
    char *string;
    ClientData data;
    char buffer[BUF_SIZE];
    if((fd = open("../FIFOS/servidor", O_WRONLY)) == -1){
                perror("erro ao abrir o fifo");
                close(fd);
                return 1;
    }
    strcpy(data.command, "status");
    // escrever a struct para o pipe
    write(fd, &data, sizeof(ClientData));
    close(fd);
    
    // escreve as tasks em modo scheduled 
    if((lista = open("../FIFOS/lista", O_RDONLY)) == -1){
                perror("erro ao abrir o fifo");
                close(lista);
                return 1;
    }
    // le do server as tasks em modo "Scheduled" e escreve para o stdout
    string = "\nScheduled\n";
    write(1, string, strlen(string));
    while (read(lista, &data, sizeof(data)) > 0) {
        int bytes_written = snprintf(buffer, sizeof(buffer), "%d %s\n", data.id, data.prog_args);
        write(1, buffer, bytes_written);
    }
    close(lista);

    if((execs = open("../FIFOS/execs", O_RDONLY)) == -1){
                perror("erro ao abrir o fifo");
                close(execs);
                return 1;
    }
    // le do server as tasks em modo "Executing" e escreve para o stdout
    string = "\n\nExecuting\n";
    write(1, string, strlen(string));
    while (read(execs, &data, sizeof(data)) > 0) {
        if(data.status == 1) {
            int bytes_written = snprintf(buffer, sizeof(buffer), "%d %s\n", data.id, data.prog_args);
            write(1, buffer, bytes_written);
        }
    }
    close(execs);

    // le as tasks terminadas alocadas no file "Tarefas_terminadas.txt" e escreve no stdout
    char *completed = "\n\nCompleted\n";
    write(1, completed, strlen(completed));
    fd = open("../Server/Output/Tarefas_terminadas.txt", O_RDONLY);
    int bytes_read;
    while ((bytes_read = read(fd, buffer, BUF_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    close(fd);
    return 0;
}
