#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) {  
        fprintf(2, "Sleep: argument error...\n");
        exit(1);
    }
    int sleep_ticks = atoi(argv[1]);
    if (sleep(sleep_ticks) == -1) {
        fprintf(2, "Sleep: process killed...\n");
        exit(1);
    }
    exit(0);
}