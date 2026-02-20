#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "stdbool.h"
#include "stddef.h"

bool DIRONLY = 0;
int LEVEL = 1e3;

char* fmtname (char* path) {
    char *p;
    for (p = path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p;
}

void print_tree_item(char* path, int cur_level, char* is_last) {
    for (int i = 1; i < cur_level; i++) {
        if (is_last[i] == 1)
            printf("     ");
        else printf("│    ");
    }
    if (cur_level > 0) {
        if (is_last[cur_level] == 1) 
            printf("└─── ");
        else printf("├─── ");
    }
    printf("%s\n",fmtname(path)); 
}

void tree (char* path, int cur_level, char* is_last) {
    if (cur_level > LEVEL)
        return;
    char buf[512];
    char *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        printf("tree: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        printf("tree: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if ((st.type == T_DEVICE || st.type == T_FILE) && !DIRONLY) {
        print_tree_item(path, cur_level, is_last);
        close(fd);
    }
    else if (st.type == T_DIR) {
        print_tree_item(path, cur_level, is_last);
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
            printf("tree: path too long\n");
        }
        else {
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            char** names = (char**)malloc(512);
            int cnt = 0;
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (!de.inum || !strcmp(de.name,".") || !strcmp(de.name,"..")) 
                    continue;
                names[cnt] = (char*)malloc(strlen(de.name)+1);
                strcpy(names[cnt], de.name);
                cnt++;
            }
            close(fd);
            for (int i = 0; i < cnt; i++) {
                strcpy(p, names[i]);
                if (i == cnt-1)
                    is_last[cur_level+1] = 1;
                tree(buf, cur_level+1, is_last);
                free(names[i]);
            }
            free(names);
        }
    }
    else {
        close(fd);
    }
}


int main(int argc, char** argv) {
    int cnt = argc-1;
    bool used_argument[cnt];
    memset(used_argument, 0, cnt);
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-d")) {
            DIRONLY = 1;
            used_argument[i-1] = 1;
            cnt--;
            break;
        }
    }
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-L")) {
            for (int j = 0; j < strlen(argv[i+1]); j++) {
                if (argv[i+1][j] < '0' || argv[i+1][j] > '9') {
                    printf("tree: invalid level");
                    exit(1);
                }
            }
            LEVEL = atoi(argv[i+1]);
            used_argument[i-1] = used_argument[i] = 1;
            cnt -= 2;
            break;
        }
    }
    if (cnt > 1) {
        printf("tree: too much argument");
        exit(1);
    }
    if (cnt < 1) {
        printf("tree: missing path argument");
        exit(1);
    }

    char* path = NULL;
    for (int i = 1; i < argc; i++) {
        if (!used_argument[i-1]) {
            path = argv[i];
        }
    }
    if (!path) exit(0);

    char* is_last = (char*)malloc(LEVEL);
    memset(is_last, 0, LEVEL);
    tree(path, 0, is_last);
    free(is_last);
    exit(0);
}