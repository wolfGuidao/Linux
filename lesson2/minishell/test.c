#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_CMD 1024
char command[MAX_CMD];

int do_face()
{
  memset(command,0,sizeof(command)/sizeof(command[0]));
  printf("minishell$");
  fflush(stdout);
  char str;
  while((str=getchar()!=EOF))
  {

  }
}

int main()
{
  return 0;
}
