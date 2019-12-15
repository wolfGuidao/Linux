#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>

#define THREAD_NUM 1

//线程安全和可重入的比较


int g_count=0;
//创建互斥锁或互斥量
pthread_mutex_t mutex;

//这个函数是线程安全，但不是可重入函数
void ModifyCount()
{
    //对临界资源加锁操作
    pthread_mutex_lock(&mutex);
    ++g_count;
    printf("befor sleep\n");
    //sleep(3)模拟程序执行临界区代码时间长的情况
    sleep(3);
    printf("after sleep\n");
    //解锁操作
    pthread_mutex_unlock(&mutex);
}

void* ThreadEntry(void* arg)
{
    (void)arg;
    for(int i=0;i<5000;i++)
    {
        ModifyCount();
    }
    return NULL;
}
void MyHandler(int sig)
{
    //信号处理函数
    (void)sig;
    printf("捕捉到SIGINT信号\n");
    ModifyCount();
}
int main()
{
    //注意他的处理机制，一旦捕捉到信号,源程序的执行逻辑处于等待状态，去处理信号函数，直到信号处理函数都处理完
    //之后，才继续处理原程序的执行逻辑
    signal(SIGINT,MyHandler);

    //对锁进行初始化
    pthread_mutex_init(&mutex,NULL);

    pthread_t tid[THREAD_NUM];
    for(int i=0;i<THREAD_NUM;i++)
    {
        pthread_create(&tid[i],NULL,ThreadEntry,NULL);
    }
    for(int i=0;i<THREAD_NUM;i++)
    {
        pthread_join(tid[i],NULL);
    }
    printf("g_count:%d\n",g_count);

    //释放锁
    pthread_mutex_destroy(&mutex);
    return 0;
}
