#include "kernel/types.h"
#include "user/user.h"

int main() {
    printf("sleeping for 100 ticks...\n");
    sleep(100);
    printf("Con ca!!\n");
    exit(0);
}