#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc > 1) {
        fprintf(2, "Pingpong: Argument error...\n");
        exit(1);
    }
    int p_pa2ch[2], p_ch2pa[2];
    pipe(p_pa2ch);
    pipe(p_ch2pa);
    char buf[4];
    if (fork() == 0) {
        int pid = getpid();
        int n = read(p_pa2ch[0], buf, sizeof(buf));
        if (n != 1) {
            fprintf(2, "Pingpong: Child receive byte error...\n");
            exit(1);
        }
        printf("%d: received ping\n", pid);
        write(p_ch2pa[1], "p", 1);
    }
    else {
        int pid = getpid();
        write(p_pa2ch[1], "p", 1);
        wait(0);
        int n = read(p_ch2pa[0], buf, sizeof(buf));
        if (n != 1) {
            fprintf(2, "Pingpong: Parent receive byte error...\n");
            exit(1);
        }
        printf("%d: received pong\n", pid);
    }
    exit(0);
}