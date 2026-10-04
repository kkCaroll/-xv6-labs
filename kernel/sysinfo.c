#include "sysinfo.h"
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "stat.h"
#include "proc.h"

uint64
sys_sysinfo(void)
{
  struct sysinfo info;
  struct proc *p = myproc();
  uint64 addr;

  if(argaddr(0, &addr) < 0)
    return -1;

  info.freemem = kfreemem();
  info.nproc = nproc();

  if(copyout(p->pagetable, addr, (char*)&info, sizeof(info)) < 0)
    return -1;
  
  return 0;
}