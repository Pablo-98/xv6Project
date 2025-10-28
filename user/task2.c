#include "user/user.h"

int main(void) {
    int pid1 = fork();
    if (pid1 == 0) {
        setpriority(getpid(), 40);
        printf("Child A (pid %d) with priority 40\n", getpid());
        while (1);
    }

    int pid2 = fork();
    if (pid2 == 0) {
        setpriority(getpid(), 39);
        printf("Child B (pid %d) with priority 39\n", getpid());
        while (1);
    }

    int pid3 = fork();
    if (pid3 == 0) {
        setpriority(getpid(), 40);
        printf("Child C (pid %d) with priority 40\n", getpid());
        while (1);
    }

    sleep(100);
    kill(pid1);
    kill(pid2);
    kill(pid3);

    exit(0);
}
