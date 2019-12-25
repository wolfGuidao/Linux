#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
  /*
  pid_t id = fork();

  //printf("@");
  if(id<0)
  {
    perror("fork error\n");
    return 1;
  }
  else if(id==0)
  {
    printf("@");
    //printf("i am child:%d\n",getpid());
  }
  else
  {
    printf("@");
    //printf("i am father:%d\n",getppid());
  }
  //printf("@");
  */

  for (int i=0;i<2;i++)
  {
    pid_t id=fork();
    (void)id;
    printf("@");
    //注意有无刷新缓冲区的区别
    fflush(stdout);
    sleep(2);
  }
  return 0;
}
