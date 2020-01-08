#include <stdio.h>
#include <signal.h>

void handler(int sig)
{
  printf("cath a signal : %d\n",sig);
}

int main()
{
  signal(2,handler);
  while(1);
  return 0;
}
