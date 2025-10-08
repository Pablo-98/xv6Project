#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

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

    int status;
    struct rusage ru;
   // wait(0);  this was for task 1

   wait2(&status, &ru);


    int end = uptime();
    int elapsed = end - start;
    int cputime = ru.cputime; 

    float percent = (float)cputime / (float)elapsed * 100.0;

    printf("elapsed time %d ticks\n", elapsed);
    printf("CPU time: %d ticks\n", cputime);
  printf("CPU usage: %.2f%%\n", percent);
    exit(0);
    



}





