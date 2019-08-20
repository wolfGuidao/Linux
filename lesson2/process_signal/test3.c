#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig)
{
  printf("catch a sig : %d\n",sig);
}

int main()
{
  signal(2,handler);
  signal(40,handler);

  sigset_t set,oldset;
  sigemptyset(&set);
  sigemptyset(&oldset);

  //把set位图置为全1
  sigfillset(&set);

  sigprocmask(SIG_BLOCK,&set,&oldset);

  getchar();

  sigprocmask(SIG_UNBLOCK,&set,NULL);

  return 0;
}
