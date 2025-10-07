#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char * argv[] ) 
{
// in case no arg
    if (argc < 2) {
        printf("usage time 1 command needs 2 args");

        exit(0);
    }

    int start = uptime();
    int pid = fork();
    
    if (pid < 0){
        printf("fork fail");
        exit(0);
    }
    else if (pid ==0){
        exec(argv[1], argv +1);
        printf(" exec %s failed\n", argv[1]);\
        exit(0);
    }

    wait(0); 

    int end = uptime();
    int elapsed = end - start;

    printf("elapsed time %d ticks\n", elapsed);
    exit(0);




}





