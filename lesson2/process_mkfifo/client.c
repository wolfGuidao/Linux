#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

int main()
{
  int wfd = open("mypipe",O_WRONLY);
  if(wfd < 0)
  {
    perror("open pipe is error");
    exit(1);
  }

  char buf[1024];
  while(1)
  {
    buf[0] = 0;
    printf("Please Enter!\n");
    fflush(stdout);
    ssize_t s = read(0,buf,sizeof(buf)-1);
    if(s <= 0)
    {
      perror("read is error");
      continue;
    }
    else if(s > 0)
    {
      buf[s] = 0;
      write(wfd,buf,sizeof(buf));
    }

  }
  close(wfd);
  return 0;
}
