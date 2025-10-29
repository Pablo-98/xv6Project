#ifndef PSTAT_H
#define PSTAT_H

#include "types.h"  // for uint64

#define NPROC 64

// We don't include proc.h here, but we define the necessary values for user space:
#define UNUSED    0
#define USED      1
#define SLEEPING  2
#define RUNNABLE  3
#define RUNNING   4
#define ZOMBIE    5

// Struct for user-visible process information
struct pstat {
  int pid;                  // Process ID
  int state;                // Process state (use the #defined values)
  uint64 size;              // Size of process memory (bytes)
  int ppid;                 // Parent process ID
  char name[16];            // Process name

  int priority;             // Priority (0-49)
  uint readytime;           // Ready to run time in ticks


};

#endif // PSTAT_H
