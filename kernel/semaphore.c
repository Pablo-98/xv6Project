#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "spinlock.h"

struct semtab semtable;

// Initialize the semaphore table and its locks
void
seminit(void)
{
  initlock(&semtable.lock, "semtable");
  for(int i = 0; i < NSEM; i++) {
    initlock(&semtable.sem[i].lock, "sem");
    semtable.sem[i].count = 0;
    semtable.sem[i].valid = 0;   // mark all as free initially
  }
}

// Allocate a semaphore entry; return its index or -1 if none free
int
semalloc(void)
{
  acquire(&semtable.lock);
  for(int i = 0; i < NSEM; i++) {
    if(semtable.sem[i].valid == 0) {
      semtable.sem[i].valid = 1;
      semtable.sem[i].count = 0;   // initial count; actual value can be set by sem_init syscall
      release(&semtable.lock);
      return i;
    }
  }
  release(&semtable.lock);
  return -1;   // no free semaphore
}

// Deallocate a semaphore entry (invalidate it)
void
semdealloc(int idx)
{
  if(idx < 0 || idx >= NSEM)
    return;

  acquire(&semtable.lock);
  semtable.sem[idx].valid = 0;
  semtable.sem[idx].count = 0;
  release(&semtable.lock);
}
