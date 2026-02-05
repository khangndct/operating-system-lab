#include "kernel/types.h"
#include "user/user.h"

int main() {
    int fd[2];
    char buffer[10];
    pipe(fd);

    if (fork() == 0) {
        // child
        int child_pid = getpid();
        read(fd[0], buffer, sizeof(buffer));
        printf("%d: receive %s\n", child_pid, buffer);

        write(fd[1], "pong", 4);

        sleep(1);
    }
    else {
        // parent
        int parent_pid = getpid();
        write(fd[1], "ping", 4);

        sleep(1);
        read(fd[0], buffer, sizeof(buffer));
        printf("%d: receive %s\n", parent_pid, buffer);        
    }

    close(fd[0]);
    close(fd[1]);
    exit(0);
}