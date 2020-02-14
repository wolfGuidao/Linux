#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <queue>

#define THREADCOUNT 4

class BlockQueue
{
    public:
        BlockQueue(int Capacity)
        {
            Capacity_ = Capacity;
            pthread_mutex_init(&QueueMutex_, NULL);
            pthread_cond_init(&ConsumeCond_, NULL);
            pthread_cond_init(&ProduceCond_, NULL);
        }

        ~BlockQueue()
        {
            pthread_mutex_destroy(&QueueMutex_);
            pthread_cond_destroy(&ConsumeCond_);
            pthread_cond_destroy(&ProduceCond_);
        }

        bool IsFull()
        {
            if(Queue_.size() == Capacity_)
            {
                return true;
            }
            return false;
        }

        int Pop(int* Data)
        {
            pthread_mutex_lock(&QueueMutex_);
            while(Queue_.empty())
            {
                pthread_cond_wait(&ConsumeCond_, &QueueMutex_);
            }
            *Data = Queue_.front();
            Queue_.pop();
            pthread_mutex_unlock(&QueueMutex_);
            pthread_cond_signal(&ProduceCond_);

            return 0;
        }

        int Push(int& Data)
        {
            pthread_mutex_lock(&QueueMutex_);
            while(IsFull())
            {
                pthread_cond_wait(&ProduceCond_, &QueueMutex_);
            }
            Queue_.push(Data);
            pthread_mutex_unlock(&QueueMutex_);
            pthread_cond_signal(&ConsumeCond_);

            return 0;
        }
    private:
        std::queue<int> Queue_;
        //定义的Queue_的容量
        size_t Capacity_;
        //互斥
        pthread_mutex_t QueueMutex_;
        //同步
        pthread_cond_t ConsumeCond_;
        pthread_cond_t ProduceCond_;
};

void* ConsumeStart(void* arg)
{
    BlockQueue* bq = (BlockQueue*)arg;
    while(1)
    {
        int Data;
        bq->Pop(&Data);
        printf("ConsumeStart [%p][%d]\n", pthread_self(), Data);
    }
    return NULL;
}

void* ProduceStart(void* arg)
{
    BlockQueue* bq = (BlockQueue*)arg;
    int i = 0;
    while(1)
    {
        bq->Push(i);
        //该执行流被打断了
        printf("ProduceStart [%p][%d]\n", pthread_self(), i);
        i++;
    }
    return NULL;
}


int main()
{
    pthread_t Con_tid[THREADCOUNT], Pro_tid[THREADCOUNT];

    int i = 0;
    int ret = -1;
    BlockQueue* bq = new BlockQueue(5);
    for(; i < THREADCOUNT; i++)
    {
        ret = pthread_create(&Con_tid[i], NULL, ConsumeStart, (void*)bq);
        if(ret != 0)
        {
            printf("create thread failed\n");
            return 0;
        }
        ret = pthread_create(&Pro_tid[i], NULL, ProduceStart, (void*)bq);
        if(ret != 0)
        {
            printf("create thread failed\n");
            return 0;
        }
    }

    for(i = 0; i < THREADCOUNT; i++)
    {
        pthread_join(Con_tid[i], NULL);
        pthread_join(Pro_tid[i], NULL);
    }

    delete bq;
    return 0;
}
