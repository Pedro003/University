/* (pipelines) n se podem usar waits pois pode criar deadlock
char cmd[N][25];
int P[N - 1][2];


for 0 < i < N
    if(i==0){
        pipe(p[i]);
        if(fork()==0){
            close(p[i][0])
            dup2(p[i][1], 1)
            close(p[i][1]);
            res = execvp(cmd[i][0],cmd[i]);
            _exit(res);
        }
        else{
            close(p[i][1]);
        }
    }
    if(i == N - 1){
        if(fork()==0){
            dup2(p[i-1][0],0)
            close(p[i-1][0])
            execvp...
            _exit()
        }else{
            close(p[i-1][0])
        }
    }
    else{
        pipe(p[i]);
        if (fork()==0){
            dup2(p[i-1][0], 0);
            close(p[i-1][0]);
            dup2(p[i][1], 1);
            close(p[i][1]);
        }else{
            close(p[i-1][0]);
            close(p[i][1]);
        }
    }


*/