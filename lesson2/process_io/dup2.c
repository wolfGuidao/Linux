#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  int fd = open("myfile",O_RDWR);
  if(fd < 0)
  {
    perror("open file is error\n");
    return -1;
  }

  close(1);
  dup2(fd,1);

  while(1)
  {
    char buf[1024]={0};
    ssize_t read_size = read(0,buf,sizeof(buf)-1);
    if(read_size < 0)
    {
      perror("read is error\n");
      break;
    }

    printf("buf : %s",buf);
    fflush(stdout);
  }
  return 0;
}
