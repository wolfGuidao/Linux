#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void Printsigset(sigset_t* set)
{
  int i = 0;
  for(;i < 32;i++)
  {
    if(sigismember(set,i))
    {
      putchar('1');
    }
    else
    {
      putchar('0');
    }
  }
  puts("");
}

int main()
{
  sigset_t s;
  sigset_t p;
  sigemptyset(&s);
  sigaddset(&s,SIGINT);
  sigprocmask(SIG_BLOCK,&s,NULL);
  while(1)
  {
    sigpending(&p);
    Printsigset(&p);
    sleep(1);
  }
  return 0;
}
