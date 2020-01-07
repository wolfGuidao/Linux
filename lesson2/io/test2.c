#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
  char  buf[1024];
  ssize_t s = read(0,buf,10);
  if(s < 0)
  {
    perror("read is error!\n");
    return -1;
  }
  //printf("%s",buf);
  write(1,buf,strlen(buf));
  
  return 0;
}
