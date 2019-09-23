#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  int fd = open("myfile",O_RDONLY);
  if(fd < 0)
  {
    perror("open file is failed!\n");
    return 1;
  }
  char buf[1024];
  while(1)
  {
    ssize_t s = read(fd,buf,11);
    if(s > 0)
    {
      printf("%s\n",buf);
    }
    else
    {
      break;
    }
  }
  close(fd);
  return 0;
}
