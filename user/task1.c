#include "user/user.h"

int main() {
    int pid;

    // Check and print initial priority
    pid = getpid();
    printf("Parent initial priority: %d\n", getpriority(pid));

    // Change priority to 17
    setpriority(pid, 17);

    // Show new priority
    printf("Parent after set: %d\n", getpriority(pid));

    // Fork to make a child process
    pid = fork();

    if (pid == 0) {  // In the child process
        printf("Child inherited priority: %d\n", getpriority(getpid()));
        printf("Let's run 'ps' to see process info\n");
        char *argv[] = {"ps", 0};
        exec("ps", argv);
    } else {
        wait(0);
    }
    exit(0);
}
