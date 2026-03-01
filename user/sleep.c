#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("sleep: requires 1 argument\n");
        exit(0);
    }
    for (int i = 0; argv[1][i] != '\0'; i++) {
        if (argv[1][i] < '0' || argv[1][i] > '9') {
            printf("sleep: the argument is not a valid positive integer\n");
            exit(0);
        }
    }
    int ticks = atoi(argv[1]);
    sleep(ticks);
    exit(0);
}