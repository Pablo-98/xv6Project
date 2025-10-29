#include "user/user.h"

int
main(void)
{
    int pid = fork();
    if (pid == 0) {
        sleep(50);  // child waits
        exec("ps", (char*[]){"ps", 0});
    } else {
        sleep(10);
        exec("ps", (char*[]){"ps", 0});
    }
    exit(0);
}
