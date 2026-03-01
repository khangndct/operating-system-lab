#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char buffer[100];

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("usage: cp src dst\n");
        exit(1);
    }

    int fd[2];
    if ((fd[0] = open(argv[1], O_RDONLY)) < 0) {
        printf("cp: cannot open %s\n", argv[1]);
        exit(1);
    }
    if ((fd[1] = open(argv[2], O_CREATE|O_WRONLY)) < 0) {
        printf("cp: error while open/create %s\n", argv[2]);
        exit(1);
    }
    
    int n;
    while ((n = read(fd[0], buffer, sizeof(buffer))) > 0) {
        if (write(fd[1], buffer, n) != n) {
            printf("cp: write error: %d\n", n);
            exit(1); 
        }
    }
    if (n < 0) {
        printf("cp: read error\n");
        exit(1);
    }

    exit(0);
}