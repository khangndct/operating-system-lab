#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "stdbool.h"
#include "stddef.h"

bool S_FLAG = 0;
bool A_FLAG = 0;

long int size_of (char* path) {
    char buf[512];
    char* p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        printf("du: cannot open %s\n", path);
        return 0;
    }
    if (fstat(fd, &st) < 0) {
        printf("du: cannot stat %s\n", path);
        close(fd);
        return 0;
    }
    long int ret = 0;
    if (st.type == T_DEVICE || st.type == T_FILE) {
        ret = st.size;
        if (!S_FLAG && A_FLAG) {
            printf("%ld %s\n", ret, path);
        }
    }
    else if (st.type == T_DIR) {
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (!de.inum || !strcmp(de.name,".") || !strcmp(de.name,"..")) 
                continue;
            strcpy(p, de.name);
            ret += size_of(buf);
        }
        if (!S_FLAG) {
            printf("%ld %s\n", ret, path);
        }
    }
    close(fd);
    return ret;
}

int main (int argc, char** argv) {
    int cnt = argc-1;
    bool used_argument[cnt];
    memset(used_argument, 0, cnt);
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-s")) {
            S_FLAG = 1;
            used_argument[i-1] = 1;
            cnt--;
            break;
        }
    }
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-a")) {
            A_FLAG = 1;
            used_argument[i-1] = 1;
            cnt--;
            break;
        }
    }
    if (cnt > 1) {
        printf("du: too much argument\n");
        exit(1);
    }
    if (cnt < 1) {
        printf("du: missing path argument\n");
        exit(1);
    }

    char* path = NULL;
    for (int i = 1; i < argc; i++) {
        if (!used_argument[i-1]) {
            path = argv[i];
        }
    }
    if (!path) exit(1);

    long int res = size_of(path);
    if (S_FLAG) {
        printf("%ld %s\n", res, path);
    }
    exit(0);
} 