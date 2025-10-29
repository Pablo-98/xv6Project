#include "user/user.h"
#include "kernel/types.h"

int
main(void)
{
    int pidA = fork();
    if (pidA == 0) {
        setpriority(getpid(), 40);
        for (;;) {
            printf("Child A ran (priority = %d, CPU ticks = %d)\n",
                   getpriority(getpid()), uptime());
            sleep(1);
        }
    }

    int pidB = fork();
    if (pidB == 0) {
        setpriority(getpid(), 39);
        for (;;) {
            printf("Child B ran (priority = %d, CPU ticks = %d)\n",
                   getpriority(getpid()), uptime());
            sleep(1);
        }
    }

    sleep(5); // delay so B gets some turns before C even exists

    int pidC = fork();
    if (pidC == 0) {
        setpriority(getpid(), 40);
        for (;;) {
            printf("Child C ran (priority = %d, CPU ticks = %d)\n",
                   getpriority(getpid()), uptime());
            sleep(1);
        }
    }

    sleep(100);  // let them run and print
    kill(pidA);
    kill(pidB);
    kill(pidC);
    exit(0);
}