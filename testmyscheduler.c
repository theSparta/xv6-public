#include "param.h"
#include "types.h"
// #include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

const int N = 1000000;
int stdout = 1;

void cpu_intensive(int x);
void io_intensive(void);
void setprio1(const int x)
{
  if(setprio(x) < 0){
    printf(stdout, "Setprio failed for Process with pid %d\n",getpid());
    exit();
  }
}

void
cpu_test(int A[], const int len,int x)
{
  int pid, i;
  printf(stdout, "Scheduler test for %d CPU-bound processes started\n",len);
  for(i = 0; i < len; i++)
  {
    pid = fork();
    if(pid == 0){
      setprio1(A[i]);
      cpu_intensive(x);
      exit();
    }
  }

  for(i=0;i<len;i++)
  {
    if(wait() < 0){
      printf(stdout,  "wait stopped early\n");
        exit();
    }
  }
  printf(stdout, "--------------------------------------------------------\n");
}

void test1()
{
    printf(stdout, "1st scheduler test started\n");
    int priority[2];
    priority[0] = 1;
    priority[1] = 100;
    cpu_test(priority,2,100);
}

void test2()
{
    int i;
    printf(stdout, "2nd scheduler test started\n");
    int priority[10];
    for(i=0;i<10;i++)
      priority[i] = (i+1)*2;

    cpu_test(priority,10,100);
}

void test3(int n)
{
  int pid,i;
  printf(stdout, "3rd scheduler test started\n");
  printf(stdout,"%d low priority CPU-bound processes with 1 high priority I/O bound process\n",n-1);

  for(i=0;i<n;i++)
  {
    pid = fork();
    if(pid == 0){
      if(i==0){
        setprio1(100);
        io_intensive();
      }
      else{
        setprio1(1);
        cpu_intensive(100);
      }
      exit();
    }
  }

  for(i=0;i<n;i++)
  {
    if(wait() < 0){
      printf(stdout, "wait stopped early\n");
        exit();
    }
  }
  printf(stdout, "--------------------------------------------------------\n");
}

void
io_intensive(void)
{
  int i;
  uint start = uptime();
  printf(stdout, "Process with pid %d and priority %d started\n",getpid(),getprio());
  for(i=0 ; i<100; i++) {
    sleep(1);
  }
  printf(stdout, "Process with pid %d completed in %d ticks\n",getpid(),uptime()-start);
  return;
}

void cpu_intensive(int x)
{
  volatile uint i;
  uint j;
  uint start = uptime();
  printf(stdout, "Process with pid %d and priority %d started\n",getpid(),getprio());
  for(j=0;j<x;j++)
    for(i=1;i < N; i++);
  printf(stdout, "Process with pid %d completed in %d ticks\n",getpid(),uptime()-start);
  return;
}

int
main(int argc, char *argv[])
{
  test1();
  test2();
  test3(5);
  exit();
}