#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* thread_run(void* arg)
{
  pthread_detach(pthread_self());
  printf("%s\n",(char*)arg);
  return NULL;
}

int main()
{
  pthread_t tid;
  int ret  = pthread_create(&tid,NULL,thread_run,(void*)"thread1 run...");
  if(ret != 0)
  {
    printf("creat thread is eroor!\n");
    return -1;
  }

  ret = 0;
  sleep(1);//这一步很关键，要让线程先分离再等待

  if(pthread_join(tid,NULL) == 0)
  {
    printf("thread wait is success!\n");
    ret = 0;
  }
  else
  {
    printf("thread wait is error!\n");
    ret = 1;
  }

  printf("ret : %d",ret);
  return 0;
}
