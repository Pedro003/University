#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = getpid();    // Obter o PID do processo atual
    pid_t ppid = getppid();  // Obter o PPID do processo pai

    printf("Meu PID: %d\n", pid);
    printf("PID do meu pai: %d\n", ppid);

    return 0;
}
