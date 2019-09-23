#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
  const char *msg0="hello printf\n";
  const char *msg1="hello fwrite\n";
  const char *msg2="hello write\n";

  printf("%s",msg0);
  fwrite(msg1,strlen(msg1),1,stdout);
  write(1,msg2,strlen(msg2));

  fork();

  return 0;
}
