#include <stdio.h>
#include <signal.h>

//volatile int g_val = 1;
int g_val = 1;

void handler(int sig)
{
  g_val = 0;
  printf("sig : %d ,g_val :%d\n",sig,g_val);
}

int main()
{
  signal(2,handler);
  while(g_val)
  {
    ;
  }
  return 0;
}
