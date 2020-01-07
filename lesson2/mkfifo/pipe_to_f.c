#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m)\
  do\
{\
  perror(m);\
  exit(EXIT_FAILURE);\
}while(0)

int main()
{
  //读取管道，写入目标文件
  int outfd = open("abc.back",O_WRONLY | O_TRUNC,0664);
  if(outfd == -1)
  {
    perror("open is error");
    exit(1);
  }

  int infd = open("tp",O_RDONLY);
  if(infd == -1)
  {
    perror("open is error");
    exit(1);
  }

  char buf[10];
  int n = read(infd,buf,10);
  if(n >0)
  {
    write(outfd,buf,n);
  }

  close(infd);
  close(outfd);
  unlink("tp");
  return 0;
}
