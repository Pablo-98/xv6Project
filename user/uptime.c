#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



void Uptime() {
 printf("uptime: %d ", uptime() );
 exit(0);
}

/*See kernel/sysproc.c for the kernel code that implements the uptime system call
(look for sys uptime), user/user.h for the C definition of uptime callable from a user
3program, and user/usys.S for the assembler code that jumps from user code into the
kernel for uptime.
*/