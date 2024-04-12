
#define SERVER "fifo_server"
#define CLIENT "fifo_client"
#define SIZE 100

typedef struct msg{
    int needle;
    int pid;
    int occurrences;
} Msg;