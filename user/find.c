#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find_file(const char *path, char *tgt_name) {
    // printf("Now in %s.\n", path);
    struct stat st;
    int fd;
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    
    char buf[512], *p;
    struct dirent de;
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("find: path too long\n");
        close(fd);
        return;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
            printf("ls: cannot stat %s\n", buf);
            continue;
        }

        if (st.type == T_FILE && strcmp(de.name, tgt_name)==0) {
            printf("%s\n", buf);
        }
        if (st.type == T_DIR && strcmp(de.name, ".")!=0 && strcmp(de.name, "..")!=0) {
            find_file(buf, tgt_name);
        }
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "Find: argument error...");
        exit(1);
    }
    find_file(argv[1], argv[2]);
    exit(0);
}
