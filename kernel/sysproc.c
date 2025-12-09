#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;
  struct proc *p = myproc();

  if(argint(0, &n) < 0)
    return -1;

  addr = p->sz;
  if (n == 0)
    return addr;

  uint64 new_sz = addr + n;
  if(new_sz < p->sz){
    return (uint64)-1;
  }
  p->sz = new_sz;
  /*old eager allocatoin, we don't call growproc right away for lazy allocatoin*/
  /*if(growproc(n) < 0)
    return -1;*/
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_freepmem(void)
{
  uint64 pages = kfreepages_count();
  return pages * PGSIZE;
}


// Temporary stub implementations for semaphore syscalls.
// You'll implement real logic later.

uint64
sys_sem_init(void)
{
  uint64 uaddr;        // user pointer to sem_t
  int pshared;
  int value;
  struct proc *p = myproc();

  // args: sem_t *sem, int pshared, unsigned int value
  if (argaddr(0, &uaddr) < 0)
    return -1;
  if (argint(1, &pshared) < 0)
    return -1;
  if (argint(2, &value) < 0)
    return -1;

  // xv6 only supports process-local semaphores
  if (pshared != 0)
    return -1;

  // get a free semaphore slot in the kernel
  int idx = semalloc();
  if (idx < 0)
    return -1;

  // initialize the kernel-side semaphore
  struct semaphore *s = &semtable.sem[idx];
  acquire(&s->lock);
  s->count = value;
  s->valid = 1;
  release(&s->lock);

  // write the index back to user space as the sem_t "handle"
  if (copyout(p->pagetable, uaddr, (char *)&idx, sizeof(idx)) < 0) {
    // if we couldn't give the handle to the user, free the slot
    semdealloc(idx);
    return -1;
  }

  return 0;
}

uint64
sys_sem_destroy(void)
{
  uint64 uaddr;
  struct proc *p = myproc();
  sem_t idx;

  // arg: sem_t *sem
  if (argaddr(0, &uaddr) < 0)
    return -1;

  // read sem index from user space
  if (copyin(p->pagetable, (char *)&idx, uaddr, sizeof(idx)) < 0)
    return -1;

  if (idx < 0 || idx >= NSEM)
    return -1;

  struct semaphore *s = &semtable.sem[idx];

  // mark it invalid under the global semtable lock
  acquire(&semtable.lock);
  if (!s->valid) {
    release(&semtable.lock);
    return -1;
  }
  s->valid = 0;
  s->count = 0;
  release(&semtable.lock);

  // wake up any sleepers so they don't wait forever
  acquire(&s->lock);
  wakeup(s);
  release(&s->lock);

  return 0;
}

uint64
sys_sem_wait(void)
{
  uint64 uaddr;
  struct proc *p = myproc();
  sem_t idx;

  // arg: sem_t *sem
  if (argaddr(0, &uaddr) < 0)
    return -1;

  // read sem index
  if (copyin(p->pagetable, (char *)&idx, uaddr, sizeof(idx)) < 0)
    return -1;

  if (idx < 0 || idx >= NSEM)
    return -1;

  struct semaphore *s = &semtable.sem[idx];

  acquire(&s->lock);

  if (!s->valid) {
    release(&s->lock);
    return -1;
  }

  // wait until count > 0
  while (s->count == 0) {
    sleep(s, &s->lock);
    // when we wake up, we hold s->lock again

    if (!s->valid) {
      // semaphore was destroyed while we slept
      release(&s->lock);
      return -1;
    }
  }

  // consume one "permit"
  s->count--;
  release(&s->lock);

  return 0;
}

uint64
sys_sem_post(void)
{
  uint64 uaddr;
  struct proc *p = myproc();
  sem_t idx;

  // arg: sem_t *sem
  if (argaddr(0, &uaddr) < 0)
    return -1;

  // read sem index
  if (copyin(p->pagetable, (char *)&idx, uaddr, sizeof(idx)) < 0)
    return -1;

  if (idx < 0 || idx >= NSEM)
    return -1;

  struct semaphore *s = &semtable.sem[idx];

  acquire(&s->lock);

  if (!s->valid) {
    release(&s->lock);
    return -1;
  }

  // give back one "permit" and wake waiters
  s->count++;
  wakeup(s);
  release(&s->lock);

  return 0;
}