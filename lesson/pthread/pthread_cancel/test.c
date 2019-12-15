#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
//创建线程
//中止线程
//等待线程
//线程分离
void* ThreadEntry(void*arg )
{
  (void)arg;
  printf("%d\n",getpid());
  int count=15;
  while(count--)
  {
    printf("In ThreadEntry,%lu\n",pthread_self());

    //pthread_exit(tid); 
    sleep(1);
  }
  pthread_exit(NULL);
  while(1)
  {
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
  pthread_create(&tid,NULL,ThreadEntry,NULL);
  /*int count=15;
  while(count--)
  {
    printf("In Main pthread\n");
    sleep(1);
  }
  pthread_cancel(tid);
  while(count--)
  {  
    printf("In Main pthread\n");  
    sleep(1);  
  }*/
  while(1)
  {
    printf("In Main Pthread\n");
    sleep(1);
  }
  return 0;
}
