#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define THREAD_NUM 2
//本意是让两个线程都对g_count加5000次，然而由于线程的抢占式调用和线程共用同一个虚拟地址空间这两个原因
//导致出现线程安全问题，即两个线程堆g_count加的次数是随机的，不确定的

//所以采用加锁来解决为题


//创建互斥锁或互斥量
pthread_mutex_t mutex;

int g_count=0;

void* ThreadEntry(void* arg)
{
    (void)arg;
    for(int i=0;i<5000;i++)
    {
        //对临界资源加锁操作
        pthread_mutex_lock(&mutex);
        ++g_count;
        printf("g_count: %d\n",g_count);
        //解锁操作
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main()
{
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
    //printf("g_count:%d\n",g_count);
    
    //释放锁
    pthread_mutex_destroy(&mutex);
    return 0;
}
