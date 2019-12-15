#include<iostream>
#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<queue>
using namespace std;
#if 1
//用vector和条件变量模拟实现一个消费者模型

pthread_mutex_t lock;
pthread_cond_t cond;

//用vector实现消费者和生产者的交易厂所
vector<int>data;

void* Product(void* arg)
{
    //生产者负责往交易场所内放资源
    (void)arg;
    int count=0;
    while(1)
    {
        //加锁
        pthread_mutex_lock(&lock);
        data.push_back(++count);
        pthread_cond_signal(&cond);
        //释放锁
        pthread_mutex_unlock(&lock);
        usleep(789789);
    }
    return NULL;
}
void* Consume(void* arg)
{
    //消费者负责从交易场所内取资源
    (void)arg;
    while(1)
    {
        //加锁
        pthread_mutex_lock(&lock);
        //每次取vector中最后一个元素
        while(data.empty())
        {
            //1.wait中第二个参数的作用就是在释放锁时用到的
            //2.等待条件就绪，即等待其他线程调用pthread_cong_signal
            //3.如果条件就绪就从新获取锁
            //4.wait的作用就是如果vector中没有数据时，消费者线程就不用进行空转，执行无用的代码，所以节省了资源
            pthread_cond_wait(&cond,&lock);
        }
        int result=data.back();
        data.pop_back();
        printf("result = %d\n",result);
        //释放锁
        pthread_mutex_unlock(&lock);
        usleep(123123);
        return NULL;
    }
}
int main()
{
    //初始化锁
    pthread_mutex_init(&lock,NULL);
    //初始化条件变量
    pthread_cond_init(&cond,NULL);

    //用两个线程模拟生产者和消费者
    pthread_t tid1,tid2;
    //生产者
    pthread_create(&tid1,NULL,Product,NULL);
    //消费者
    pthread_create(&tid2,NULL,Consume,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    //销毁锁
    pthread_mutex_destroy(&lock);
    //销毁条件变量
    pthread_cond_destroy(&cond);

    return 0;
}
#endif

//使用队列和信号量来模拟生产者和消费者
//信号量就是一个计数器
//P申请资源，计数器-1
//V释放资源，计数器+1
#if 0
#include<semaphore.h>

sem_t sem;

int main()
{
    //初始化一个信号量
    //int sem_init(sem_t* sem,int pshared,unsigned int val)
    //第二个参数pshared如果为0，表示这个信号量只能在当前进程内使用
    //第二个参数如果为非0，表示这个信号量可以在进程间共享
    //第三个参数表示信号量计数器的初始值
    sem_init(&sem,0,1);

    //P
    sem_wait(&sem);
    printf("P操作一次\n");
    sem_wait(&sem);
    printf("P操作一次\n");
    //V
    //sem_post(&sem);

    //销毁一个信号量
    sem_destroy(&sem);
    return 0;
}
#endif
