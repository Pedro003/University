//faltam os includes 
#define BUFF_SIZE 1024

int main(){

    //in
    int fd_in = open("/etc/passwd", O_RDONLY);
    if(fd_in == -1){
        perror("erro no open do fd_in");
        return -1;
    }

    int res = dup2(fd_in, 0);
    if(res == -1){
        perror("erro no dup2 do fd_in");
        return -1;
    }
    printf("dup2 in = %d\n", res);

    close(fd_in);

    //out
    int fd_out = open("saida.txt", O_WRONLY | O_CREATE | O_TRUNC, 0666);
    if(fd_out == -1){
        perror("erro no open do fd_out");
        return -1;
    }

    int fd_out_original = fd_out;
    res = dup2(fd_out, 1);
    if(res == -1){
        perror("erro no dup2 do fd_out");
        return -1;
    }
    printf("dup2 out = %d\n", res);

    close(fd_out);

    //err
    int fd_err = open("erros.txt", O_WRONLY | O_CREATE | O_TRUNC, 0666);
    if(fd_out == -1){
        perror("erro no open do fd_out");
        return -1;
    }

    res = dup2(fd_err, 2);
    if(res == -1){
        perror("erro no dup2 do fd_err");
        return -1;
    }
    printf("dup2 err = %d\n", res);

    close(fd_err);

    char buf[BUFF_SIZE];
    int bytes_read;

    while ((bytes_read = read(0,buf,BUFF_SIZE)) > 0){
        write(1, buf, bytes_read);
        write(2, buf, bytes_read);
    }

    dup2(fd_out_original , 1);

    printf("terminei\n");




    return 0;
}