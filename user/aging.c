#include "user/user.h"

void hog() {
  while (1); // Infinite loop to hog CPU
}

int
main(void)
{
  int pid = fork();
  if (pid == 0) {
    // child: low priority, should get starved initially but later age in
    setpriority(getpid(), 0);
    printf("[LOW] child started with priority 0\n");
    hog();
  } else {
    // parent: high priority hog
    setpriority(getpid(), 49);
    printf("[HIGH] parent hogging CPU at priority 49\n");
    hog();
  }
  exit(0);
}
