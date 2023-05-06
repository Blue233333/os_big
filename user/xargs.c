#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(2, "xargs: too few arguments...\n");
        exit(1);
    }

    char buf[512], *sub_argv[MAXARG], c;
    int buf_p = 0, n, sub_argc = argc;
    for (int i = 0; i < argc; i++) {
        sub_argv[i] = argv[i];
    }
    while ((n = read(0, &c, 1)) != 0) {
        if (c == ' ' || c == '\n') {
            sub_argv[sub_argc] = (char *)malloc(sizeof(char) * buf_p);
            memmove(sub_argv[sub_argc], buf, buf_p);
            sub_argc++;
            buf_p = 0;
            if (c == '\n') {
                if (fork() == 0) {
                    sub_argv[sub_argc] = 0;
                    exec(sub_argv[0], sub_argv);
                }
                else {
                    for (int i = argc; i < sub_argc; i++) {
                        free(sub_argv[i]);
                    }
                    sub_argc = argc;
                    wait(0);
                }
            }
        }
        else {
            buf[buf_p++] = c;
            continue;
        }
    }
    exit(0);
}