#include "param.h"
#include "types.h"
// #include "stat.h"
#include "user.h"
// #include "fs.h"
// #include "fcntl.h"
#include "syscall.h"
#include "traps.h"

#define N 1000000000

void test(int i);
// meant to be run w/ at most two CPUs
void
schedulertest(void)
{
  int pid, i;

  printf(1, "scheduler test\n");

  for(i = 0; i < 2; i++)
  {
    pid = fork();
    if(pid == 0){
      test(i+1);
      exit();
    }
  }

  for(i=0;i<2;i++)
  {
    if(wait() < 0){
      printf(1, "wait stopped early\n");
        exit();
    }
  }
}

void test(int x)
{ 
  if(setprio(x) < 0){
    printf(2,"Setprio failed for Process with pid %d\n",getpid());
    exit();
  }
  uint i;
  uint start = uptime(); 
  printf(1,"Process with pid %d and priority %d started\n",getpid(),getprio());
    for(i=0;i < N; i++)
    {
      if( (i % N/2) == 0)
        printf(1,"Process with pid %d running\n",getpid());
    }
    printf(1,"Process with pid %d completed in %d\n cycles\n",getpid(),uptime()-start);
    return;
}

int
main(int argc, char *argv[])
{
  
  schedulertest();
  exit();
}