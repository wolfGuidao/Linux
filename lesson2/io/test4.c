#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  close(1);
  int fd = open("myfile",O_WRONLY);
  if(fd < 0)
  {
    perror("open file is error\n");
    return -1;
  }
  printf("fd = %d",fd);
  fflush(stdout);
  close(fd);
  exit(0);
  return 0;
}
