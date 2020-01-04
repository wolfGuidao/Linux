#include <iostream>
#include <sys/wait.h>
#include <cstdio>
#include <unistd.h>
using namespace std;

int main()
{
  pid_t pid = fork();
  if(pid < 0)
  {
    perror("fork error!\n");
    return -1;
  }
  else if(pid == 0)
  {
    //child
    //printf("i am child!\n");
    
    //while(1)
     printf("i am child\n");
     sleep(10);
  }
  else
  {
    //father
    //while(1)
    //printf("wolf!\n");
    
    printf("i am father\n");

#if 0
    int ret = wait(NULL);
    if(ret==-1)
    {

      perror("wait is error!\n");
      return -1;
    }
    else
    {
      printf("pid = %d\n",ret);
    }

#endif

    //int ret = waitpid(pid,NULL,0);
    ///int ret = waitpid(0,NULL,WNOHANG);
    int ret = waitpid(-1,NULL,0);
    if(ret == -1)
    {
      perror("waitpid is error!\n");
      return -1;
    }
    if(ret == 0)
    {
      printf("WNOHANG\n");
    }
    else
    {
      printf("pid= %d\n",ret );
    }

  }
  return 0;
}
