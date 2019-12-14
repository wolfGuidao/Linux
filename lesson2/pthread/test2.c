#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* thread1(void* arg)
{
  (void)arg;
  printf("thread1 returning...\n");
  int* p = (int*)malloc(sizeof(int));
  *p = 1;
  return (void*)p;
}

void* thread2(void* arg)
{
  (void)arg;
  printf("thread2 returning...\n");
  int* p = (int*)malloc(sizeof(int));
  *p = 2;
  pthread_exit((void*)p);
  return NULL;
}

void* thread3(void* arg)
{
  (void)arg;
  while(1)
  {
    printf("thread3 returning...\n");
    sleep(1);
  }
  return NULL;

}

int main()
{
  pthread_t tid;
  void* ret;

  //thread1 return 
  pthread_create(&tid,NULL,thread1,NULL);
  pthread_join(tid,&ret);
  printf("thread1 return,thread id %lu,return code:%d\n",tid,*(int*)ret);

  //thread2 exit 
  pthread_create(&tid,NULL,thread2,NULL);
  pthread_join(tid,&ret);
  printf("thread2 return,thread id %lu,return code:%d\n",tid,*(int*)ret);
  free(ret);

  //thread3 cancel by other 
  pthread_create(&tid,NULL,thread3,NULL);
  sleep(3);
  pthread_cancel(tid);
  pthread_join(tid,&ret);
  if(ret == PTHREAD_CANCELED)
  {
    printf("thread3 is return,thread id %lu,return code:PTHREAD_CANCELED\n",tid);
  }
  else
  {
    printf("thread3 is return,thread id %lu,return code:NULL\n",tid);
  }
  return 0;
}
