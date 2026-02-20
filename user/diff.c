#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "stdbool.h"
#include "stddef.h"

bool Q_FLAG = 0;

char* fmtname (char* path) {
    char* p;
    for (p = path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p;
}

void strip_tail(char* str, char* delimiter) {
    if (!strlen(str)) return;
    char* p = str+strlen(str)-1;
    *p = '\0';
}

int readline(int fd, char *buf, int max) {
    int n = 0;
    char c;
    while (n < max - 1) {
        int rc = read(fd, &c, 1);
        if (rc < 1) return n; // EOF or error
        buf[n++] = c;
        if (c == '\n') break;
    }
    buf[n] = '\0';
    return n;
}

void diff(char* path1, char* path2) {
    int fd1, fd2;
    if ((fd1 = open(path1, O_RDONLY)) < 0) {
        printf("diff: cannot open %s\n", path1);
        return;
    }
    if ((fd2 = open(path2, O_RDONLY)) < 0) {
        printf("diff: cannot open %s\n", path2);
        close(fd1);
        return;
    }

    char* file1 = fmtname(path1);
    char* file2 = fmtname(path2);

    char buf1[1024];
    char buf2[1024];

    bool identical = true;
    int cnt = 1;
    
    while(1) {
        int n1 = readline(fd1, buf1, 1024);
        int n2 = readline(fd2, buf2, 1024);
        if (n1 < 0 || n2 < 0) {
            printf("diff: read error\n");
            close(fd1);
            close(fd2);
            return;
        }
        strip_tail(buf1, "\n");
        strip_tail(buf2, "\n");
        if (!n1 && !n2) break;
        if (n1 != n2 || memcmp(buf1, buf2, n1)) {
            identical = false;
            if (Q_FLAG) {
                printf("diff: files differ\n");
                close(fd1);
                close(fd2);
                return;
            }
            buf1[n1] = 0;
            buf2[n2] = 0;
            if (n1 == 0) strcpy(buf1, "EOF");
            if (n2 == 0) strcpy(buf2, "EOF");
            printf("%s:%d: < %s\n", file1, cnt, buf1);
            printf("%s:%d: > %s\n", file2, cnt, buf2);
        }
        cnt++;
    }

    if (identical) printf("diff: files are identical");
    close(fd1);
    close(fd2);
}

int main(int argc, char** argv) {
    int cnt = argc-1;
    bool used_argument[cnt];
    memset(used_argument, 0, cnt);
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-q")) {
            Q_FLAG = 1;
            used_argument[i-1] = 1;
            cnt--;
            break;
        }
    }
    if (cnt > 2) {
        printf("diff: too much argument\n");
        exit(1);
    }
    if (cnt < 2) {
        printf("diff: missing path argument\n");
        exit(1);
    }

    char* path1 = NULL;
    char* path2 = NULL;
    
    for (int i = 1; i < argc; i++) {
        if (!used_argument[i-1]) {
            if (!path1) path1 = argv[i];
            else path2 = argv[i];
        }
    }

    if (!path1 || !path2) exit(1);

    diff(path1, path2);

    exit(0);
}