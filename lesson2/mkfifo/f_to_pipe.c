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
  //读取文件，写入命名管道
  mkfifo("tp",0664);
  int infd = open("abc", O_RDONLY);
  if(infd == -1)
  {
    //ERR_EXIT("open");
    perror("open is error");
    exit(0);
  }

  int outfd = open("tp",O_WRONLY);
  if(outfd == -1)
  {
    //ERR_EXIT("open");
    perror("open is error");
    exit(1);
  }

  char buf[10];
  int n = read(infd,buf,10);
  if(n > 0 ) 
  {
    write(outfd,buf,n);
  }

  close(infd);
  close(outfd);
  
  return 0;
}
