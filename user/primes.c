#include "kernel/types.h"
#include "user/user.h"

void seive(int read_fd) {
    int prime;
    if(read(read_fd, &prime, sizeof(int)) <= 0) {
        close(read_fd);
        return;
    }
    printf("prime %d\n", prime);

    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        close(fd[1]);
        close(read_fd);
        seive(fd[0]);
    }
    else {
        close(fd[0]);
        int num;
        while (read(read_fd, &num, sizeof(int)) > 0) {
            if (num % prime != 0) {
                write(fd[1], &num, sizeof(int));
            }
        }
        close(fd[1]);
        close(read_fd);
    }
    wait(0);
}

int main () {
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        close(fd[1]);
        seive(fd[0]);
    }
    else {
        close(fd[0]);
        for (int i = 2; i <= 280; i++) {
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
    }
    wait(0);
    exit(0);
}
