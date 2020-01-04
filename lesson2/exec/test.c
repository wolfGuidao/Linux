#include <stdio.h>
#include <unistd.h>

int main()
{
  //execl("/usr/bin/ls","ls","-a",NULL);
  //printf("wolf\n");

  //execlp("ls","ll",NULL);
  //printf("wolf\n");

  //execlp("xxls","ll",NULL);
  //printf("wolf\n");
  
  char* const envp[]={"PATH=/home/dev/hello.c",NULL};
  //execle("/usr/bin/ls","ls",NULL,envp);
  //execle("/home/dev/hello","hello",NULL,envp);
  //printf("wolf\n");
  
  //char* const argv[]={"ls","-a","-l",NULL};
  //execv("/usr/bin/ls",argv);
  
  //char* const argv[]={"ls","-a","-l",NULL};
  //execvp("ls",argv);
  
  char* const argv[]={"ls","-a","-l",NULL};
  execve("/usr/bin/ls",argv,envp);

  return 0;
}
