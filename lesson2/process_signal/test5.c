#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int g_val = 0;

void handler(int sig)
{
  g_val++;
  printf("sig :%d\n",sig);
}

int main()
{
  signal(2,handler);
  int count = 100;
  while(count--)
  {
    g_val++;
    printf("count: %d\n",count);
    //sleep(100000);
    sleep(1);
  }
  printf("g_val: %d\n",g_val);
  return 0;
}
