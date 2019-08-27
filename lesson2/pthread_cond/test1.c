#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

pthread_cond_t cond;//定义一个条件变量

pthread_mutex_t mutex;//定义一个互斥量

void* r1(void* arg)
{
  (void)arg;
  while(1)
  {
    pthread_cond_wait(&cond,&mutex);
    printf("活动！\n");
  }
  return NULL;
}

void* r2(void* arg)
{
  (void)arg;
  while(1)
  {
    pthread_cond_signal(&cond);
    sleep(1);
  }
  return NULL;
}

int main()
{
  pthread_t t1,t2;

  pthread_cond_init(&cond,NULL);//初始化条件变量
  pthread_mutex_init(&mutex,NULL);//初始化互斥量

  pthread_create(&t1,NULL,r1,NULL);
  pthread_create(&t2,NULL,r2,NULL);

  pthread_join(t1,NULL);
  pthread_join(t2,NULL);

  pthread_cond_destroy(&cond);//销毁条件变量
  pthread_mutex_destroy(&mutex);//销毁互斥量

  return 0;
}
