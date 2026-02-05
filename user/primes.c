#include "kernel/types.h"
#include "user/user.h"

int main() {
    char buffer[281];
    for (int i = 2; i <= 280; i++) buffer[i] = '1'; // initialize buffer

    int fd[2];
    pipe(fd);

    for (int i = 2; i <= 280; i++) {
        if (fork() == 0) {
            read(fd[0], buffer, 281);
            if (buffer[i] == '1') {
                printf("prime %d\n", i);
                for (int j = i*i; j <= 280; j += i) buffer[j] = '0';
                write(fd[1], buffer, 281);
                close(fd[0]);
                close(fd[1]);
                pipe(fd);
            }
        }
        else {
            close(fd[0]);
            write(fd[1], buffer, 281);
            close(fd[1]);
            exit(0);
        }
    }
    
    exit(0);
}
