#include "kernel/types.h"
#include "user/user.h"

int main() {
    int fd[2];
    char buffer[1];
    pipe(fd);

    if (fork() == 0) {
        // child
        int child_pid = getpid();
        read(fd[0], buffer, 1);
        printf("%d: receive ping\n", child_pid);

        write(fd[1], "o", 1);

        sleep(1);
    }
    else {
        // parent
        int parent_pid = getpid();
        write(fd[1], "o", 1);

        sleep(1);
        read(fd[0], buffer, 1);
        printf("%d: receive pong\n", parent_pid);        
    }

    close(fd[0]);
    close(fd[1]);
    exit(0);
}