#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

extern void* spalloc();
extern void spfree(void* ptr);

void*
sys_spalloc()
{  
  return (void*)spalloc();
}

int
sys_spfree()
{
  int addrSp;
  if(argint(0,&addrSp) < 0)
  {
    return 0;	  
  }
  int* addrSpPtr = (int*)addrSp;

  spfree(addrSpPtr); 
  return 0;
}

extern int mutex_init(mutex_t*);
extern int mutex_lock(mutex_t*);
extern int mutex_unlock(mutex_t*);


int sys_mutex_init(void)
{
  int addrSp;
  if(argint(0,&addrSp) < 0)
  {
    return -1;	  
  }
  int* addrSpPtr = (int*)addrSp;

  mutex_init((mutex_t*)addrSpPtr);
   
  return 0;	
}

int sys_mutex_lock(void)
{
  int addrSp;
  if(argint(0,&addrSp) < 0)
  {
    return -1;	  
  }
  int* addrSpPtr = (int*)addrSp;

  mutex_lock((mutex_t*)addrSpPtr);
  return 0;	
}

int sys_mutex_unlock(void)
{ 
  int addrSp;
  if(argint(0,&addrSp) < 0)
  {
    return -1;	  
  }
  int* addrSpPtr = (int*)addrSp;

  mutex_unlock((mutex_t*)addrSpPtr);
 
  return 0;	
}

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_halt()
{
  // Incantation from Redox OS
  outw(0x604, 0x2000);
  while(1);
  return 0;
}
