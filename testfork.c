#include"unistd.h"
#include"stdio.h"
int main()
{
int pid;
pid=fork();
switch(pid)
{
case -1:
  printf("fork is faild");
case 0:
{
     execl("/bin/ls","ls","-al",NULL);
     perror("execl failed");
     exit(0);
}
default:
{
  wait(NULL);
  printf("parent finish\n");
  exit(1);
}
}
return 0;
}
