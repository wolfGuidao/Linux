#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig)
{
  printf("catch a sig : %d \n",sig);
}

int main()
{
  signal(SIGSEGV,handler);
  sleep(1);
  int* p = NULL;
  *p =100;
  while(1)
  {
    ;
  }
  return 0;
}
