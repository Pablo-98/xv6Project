#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("Usage: pexec <priority> <command> [args...]\n");
    exit(0);
  }

  int priority = atoi(argv[1]);
  if (priority < 0 || priority > 49) {
    printf("Priority out of range (0–49)\n");
    exit(1);
  }

  int pid = fork();
  if (pid == 0) {
    setpriority(getpid(), priority);
    exec(argv[2], &argv[2]);
    printf("pexec: exec %s failed\n", argv[2]);
    exit(1);
  }

  wait(0);
  exit(0);
}
