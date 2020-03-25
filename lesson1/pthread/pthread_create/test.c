#include<stdio.h>
#include <iostream>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
//创建线程
//中止线程
//等待线程
//线程分离
void* test(void*arg)
{
  (void)arg;
  while(1)
  {
    printf("pthread2,%lu\n",pthread_self());
    sleep(1);
  }
}
void* ThreadEntry(void*arg )
{
  (void)arg;
  pthread_t tid2;
  printf("%d\n",getpid());
  while(1)
  {
    pthread_create(&tid2,NULL,test,NULL);
    printf("In ThreadEntry,%lu\n",pthread_self());
    sleep(1);
  }
  return NULL;
}

int main()
{
  //创建线程
  //pthread_t* thread输出型参数，代表线程id
  //const pthread_attr_t* attr线程属性，默认为nullptr
  //void*(*start_routine)(void* )线程启动后要执行的函数
  //void* arg传给线程启动的函数参数
  //int pthread_creat(pthread_t* pthread,const pthread_attr_t* attr,void*(*start_routine)(void*),void* arg);
  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_create(&tid,&attr,ThreadEntry,NULL);
  size_t stack_size;
  pthread_attr_getstacksize(&attr,&stack_size);
  std::cout<<"stack size is : "<<stack_size<<std::endl;
  while(1)
  {
    printf("In Main Thread,%lu\n",pthread_self());
    sleep(1);
  }
  return 0;
}
