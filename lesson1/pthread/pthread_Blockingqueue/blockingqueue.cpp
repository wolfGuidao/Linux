#include"blockingqueue.hpp"
#include<stdio.h>
#include<unistd.h>

//因为Blocking queue是自己封装的线程安全的同步互斥，自己就不用再继续实现了

//生产者和消费者的交易场所
Blockingqueue<int>queue(100);

//生产者的操作
void* Product(void* arg)
{
    (void)arg;
    int count=0;
    while(1)
    {
        queue.Push(++count);
        usleep(789789);
    }
    return NULL;
}

//消费者的操作
void* Consume(void* arg)
{
    (void)arg;
    while(1)
    {
        int count=0;
        queue.Pop(&count);
        printf("count = %d\n",count);
        usleep(123123);
    }
    return NULL;
}

int main()
{
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,Product,NULL);
    pthread_create(&tid2,NULL,Consume,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    return 0;

}

