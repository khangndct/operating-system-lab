#include "kernel/types.h"
#include "user/user.h"

int main() {
    int fd1[2];
    int fd2[2];
    char buf[1];
    pipe(fd1);
    pipe(fd2);

    if (fork() == 0) { // child
        int child_pid = getpid();
        close(fd1[1]); // close write of fd1
        close(fd2[0]); // close read of fd2
        read(fd1[0], buf, 1);
        if (buf[0] == 'o')
            printf("%d: received ping\n", child_pid);
        write(fd2[1], "o", 1);
        close(fd1[0]);
        close(fd2[1]);
    }
    else { // parent
        int parent_pid = getpid();
        close(fd1[0]); // close read of fd1
        close(fd2[1]); // close write of fd2
        write(fd1[1], "o", 1);
        read(fd2[0], buf, 1);
        if (buf[0] == 'o')
            printf("%d: received pong\n", parent_pid);
        close(fd1[1]);
        close(fd2[0]);
    }
}