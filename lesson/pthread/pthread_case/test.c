#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
//1.线程间能够共享线程地址空间
#if 0
int g_count=0;
void* ThreadEntry(void*arg )
{
    (void)arg;
    printf("%d\n",getpid());
    while(1)
    {
        printf("In ThreadEntry,%lu\n",pthread_self());
        ++g_count;
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,ThreadEntry,NULL);
    pthread_detach(tid);
    while(1)
    {
        printf("In Main Pthread   %d\n",g_count);
        sleep(1);
    }
    return 0;
}
#endif


#if 0
//2.线程异常终止的情况
void* ThreadEntry(void* arg)
{
    (void)arg;
    while(1)
    {
        printf("In ThreadEntry pthread\n");
        int* p=NULL;
        *p=10;
        sleep(1);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,ThreadEntry,NULL);
    while(1)
    {
        printf("In Pthread Main\n");
        sleep(1);
    }
    return 0;
}
#endif

#if 0
//3.多线程利用多核资源
void* ThreadEntry(void* arg)
{
    //让两个线程同时执行死循环，让CPU的利用率超过100%
    (void)arg;
    while(1);
    return NULL;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,ThreadEntry,NULL);

    while(1);

    return 0;
}
#endif

//4.通过多线程提高程序执行效率
#define SIZE 100000000
#include<malloc.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<sys/time.h>
void Calc(int* arr,int beg,int end)
{
    int i=0;
    for(i=beg;i<end;i++)
    {
        arr[i]=arr[i]*arr[i];
    }
}

int64_t GetUs()
{
    //获取微妙
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000000+tv.tv_usec;
}
#if 0
int main()
{
    //单线程测试时间
    srand(time(NULL));
    int* arr =(int*)malloc(sizeof(int) * SIZE);
    //记录开始时间
    int64_t beg=GetUs();
    Calc(arr,0,SIZE);
    //记录结束时间
    int64_t end=GetUs();
    //两个时间做差，求执行时间
    printf("time:%ld\n",end-beg);
    return 0;
}

#endif

//宏定义线程的个数
#define THREAD_NUM 3
//多线程测试时间
//让线程分别执行一部分


//定义一个结构体方便给ThradEntry进行传参
typedef struct arg
{
    int begin;
    int end;
    int* arr;
}Arg;
void* ThreadEntry(void* arg)
{
    (void)arg;
    Arg* p=(Arg*)arg;

    //调用函数进行运算
    Calc(p->arr,p->begin,p->end);
    return NULL;
}
int main()
{
    //定义一个数组，进行运算
    int* arr=(int*)malloc(sizeof(int)*SIZE);

    //定义一个tid数组，方便创建多个线程
    pthread_t tid[THREAD_NUM];

    //为每个线程分配不同的计算任务
    Arg arg[THREAD_NUM];
    int base=0;
    for(int i=0;i<THREAD_NUM;i++)
    {
        arg[i].begin=base;
        arg[i].end=base+SIZE/THREAD_NUM;
        arg[i].arr=arr;
        base+=SIZE/THREAD_NUM;
    }

    //获取测试程序开始时间
    int64_t beg=GetUs();

    //循环创建线程
    for(int i=0;i<THREAD_NUM;i++)
    {
        //注意创建线程的第4个参数
        pthread_create(&tid[i],NULL,ThreadEntry,&arg[i]);
    }
    //循环回收等待线程
    for(int i=0;i<THREAD_NUM;i++)
    {
        pthread_join(tid[i],NULL);
    }
    //获取程序执行完毕的时间
    int64_t end=GetUs();

    //打印整个程序在多个线程下的运行时间
    printf("time:%ld\n",end-beg);
    return 0;
}
