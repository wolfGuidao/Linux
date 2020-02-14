#include <semaphore.h>
#include <pthread.h>
#include <cstdio>
#include <iostream>
#include <vector>

#define SIZE 1
#define THREADCOUNT 4

class RingQueue
{
    public:
        RingQueue()
            :Vec_(SIZE)
        {
            Capacity_ = SIZE;
            PosWrite_ = 0;
            PosRead_ = 0;

            //初始化同步的信号量
            //生产者生产的时候就看我们有多少空间可以供我们去生产
            sem_init(&ProSem_, 0, SIZE);
            //消费者初始化的时候是看当前有多少资源可以消费
            sem_init(&ConSem_, 0, 0);
            //初始化互斥
            sem_init(&LockSem_, 0, 1);
        }

        ~RingQueue()
        {
            sem_destroy(&ProSem_);
            sem_destroy(&ConSem_);
            sem_destroy(&LockSem_);
        }

        void Push(int& Data)
        {
            //对于加锁而言，由于sem_wait并没有通知LockSem进行释放资源的操作，sem_wait(&ProSem_)内部并没有 sem_post(&LockSem_)功能
            //如果现在需要往vec_里面插入数据的时候
            //当时空间已经全部被插入了，没有空间了
            //卡在了sem_wait(&ProSem_);阻塞等待 消费者来通知进行生产
            //但是这会儿由于生产者把锁资源拿着，促使消费者想要消费数据的时候，拿不到
            //LockSem_资源
            sem_wait(&ProSem_);

            sem_wait(&LockSem_);
            Vec_[PosWrite_] = Data;
            PosWrite_ = (PosWrite_ + 1) % Capacity_;
            sem_post(&LockSem_);

            //资源进行+1操作，并且唤醒消费者
            sem_post(&ConSem_);
        }

        void Pop(int* Data)
        {
            sem_wait(&ConSem_);

            sem_wait(&LockSem_);
            *Data = Vec_[PosRead_];
            PosRead_ = (PosRead_ + 1) % Capacity_;
            sem_post(&LockSem_);

            sem_post(&ProSem_);
        }
    private:
        std::vector<int> Vec_;
        size_t Capacity_;
        int PosWrite_;
        int PosRead_;
        //同步功能的信号量
        //生产者的信号量
        sem_t ProSem_;
        //消费者的信号量
        sem_t ConSem_;
        //实现互斥
        sem_t LockSem_;
};

void* ProStart(void* arg)
{
    RingQueue* rq = (RingQueue*)arg;
    int i = 0;
    while(1)
    {
        rq->Push(i);
        printf("ProStart make data [%p][%d]\n", pthread_self(), i);
        i++;
    }
    return NULL;
}

void* ConStart(void* arg)
{
    RingQueue* rq = (RingQueue*)arg;
    int Data;
    while(1)
    {
        rq->Pop(&Data);
        printf("ConStart consume data [%p][%d]\n", pthread_self(), Data);
    }
    return NULL;
}


int main()
{
    pthread_t Pro_tid[THREADCOUNT], Con_tid[THREADCOUNT];
    int i = 0;
    int ret = -1;
    RingQueue* rq = new RingQueue();
    for(; i < THREADCOUNT; i++)
    {
        ret = pthread_create(&Pro_tid[i], NULL, ProStart, (void*)rq);
        if(ret != 0)
        {
            printf("create thread failed\n");
            return 0;
        }
        ret = pthread_create(&Con_tid[i], NULL, ConStart, (void*)rq);
        if(ret != 0)
        {
            printf("create thread failed\n");
            return 0;
        }
    }

    for(i = 0; i < THREADCOUNT; i++)
    {
        pthread_join(Pro_tid[i], NULL);
        pthread_join(Con_tid[i], NULL);
    }
    delete rq;
    return 0;
}
