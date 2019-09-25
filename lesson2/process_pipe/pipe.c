#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  int fds[2];
  char buf[100];
  int len;

  if(pipe(fds) == -1)
  {
    perror("make pipe is error\n");
    return -1;
  }

  //从标准输入读数据
  while(fgets(buf,100,stdin))
  {
    len = strlen(buf);

    //往管道中写数据
    if(write(fds[1],buf,len) !=len)
    {
      perror("write into pipe is error");
      break;
    }

    memset(buf,0x00,sizeof(buf));

    //从管道中往出写数据
    if(read(fds[0],buf,100) != len)
    {
      perror("read into pipe is error");
      break;
    }

    //把从管道中读取的数据输出到标准输出
    if(write(1,buf,len) != len)
    {
      perror("write to stdout is error");
      break;
    }
  }
  return 0;
}
