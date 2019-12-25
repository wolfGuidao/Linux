#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* rout(void* arg)
{
  (void)arg;
  while(1)
  {
    printf("i am pthread child!\n");
    sleep(1);
  }

}

int main()
{
  pthread_t tid;
  int ret = pthread_create(&tid,NULL,rout,NULL);
  if(ret != 0)
  {
    fprintf(stderr,"pthread_create: %s \n",strerror(ret));
    exit(-1);
  }

  while(1)
  {
    printf("I am pthread main,tid :%lu \n",tid);
    sleep(1);
  }
  return 0;
}
