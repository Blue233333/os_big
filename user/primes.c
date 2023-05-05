#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int new_prime_filter(int p[2]) {
    if (fork()==0) {
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        
        int n, x, current_prime = -1, has_child = 0;
        int new_p[2];
        while ((n = read(0, &x, 4)) != 0) {
            // printf("%d %d\n", getpid(), x);
            if (current_prime == -1) {
                current_prime = x;
                printf("prime %d\n", current_prime);
            }
            else if (x % current_prime == 0) {}
            else {
                if (has_child == 0) {
                    pipe(new_p);
                    if (new_prime_filter(new_p) == 0) {
                        exit(0);
                    }
                    has_child = 1;
                }
                write(new_p[1], &x, 4);
            }
        }
        if (has_child == 1) {
            close(new_p[1]);
            printf("%d waiting...\n", getpid());
            wait(0);
        }
        exit(0);
    }
    else {
        return 1;
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    new_prime_filter(p);
    for (int x = 2; x <= 35; x++) {
        write(p[1], &x, 4);
    }
    close(p[1]);
    wait(0);
    exit(0);
}