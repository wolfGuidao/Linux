#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main()
{
  umask(0);
  if(mkfifo("mypipe",0664) < 0)
  {
    perror("make fifo is error");
    exit(1);
  }

  int rfd = open("mypipe",O_RDONLY);
  if(rfd < 0)
  {
    perror("open pipe is perror");
    exit(1);
  }

  char buf[1024];
  while(1)
  {
    buf[0] = 0;
    printf("please wait ...!\n");
    ssize_t s = read(rfd,buf,sizeof(buf)-1);
    if(s > 0)
    {
      buf[s-1] = 0;
      printf("client say: %s",buf);

    }
    else if (s == 0)
    {
      printf("client is quit!\n");
      exit(1);
    }
    else
    {
      perror("read is orrer");
      exit(1);
    }
  }
  close(rfd);
  return 0;
}
