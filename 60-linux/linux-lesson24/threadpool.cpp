#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>

#define THREADCOUNT 4

typedef void (*Handler_t)(int);
class ThreadTask
{
    public:
        ThreadTask()
        {
            Data_ = -1; 
            Handler_ = NULL;
        }

        ThreadTask(int Data, Handler_t Handler)
        {
            Data_ = Data;
            Handler_ = Handler;
        }

        void Run()
        {
            Handler_(Data_);
        }
    private:
        int Data_;
        Handler_t Handler_;
};

class ThreadPool
{
    public:
        ThreadPool()
        {
            ThreadCapacity_ = THREADCOUNT;
            //比较简陋的一种做法，比较好的做法是，创建线程的时候时候进行统计，然后在赋值
            ThreadCurNum_ = THREADCOUNT;

            pthread_mutex_init(&Mutex_, NULL);
            pthread_cond_init(&Cond_, NULL);
            IsQuit_ = false;

            //创建线程
            bool IsCreate = ThreadCreate();
            if(!IsCreate)
            {
                printf("ThreadPool Create thread failed\n");
                exit(1);
            }
        }

        ~ThreadPool()
        {
            pthread_mutex_destroy(&Mutex_);
            pthread_cond_destroy(&Cond_);
        }

        //放数据
        bool Push(ThreadTask* Tt)
        {
            pthread_mutex_lock(&Mutex_);
            //如果设置了线程池当中的线程进行退出，则就不要再进行push数据了，将互斥锁释放，让线程池当中的线程退出
            if(IsQuit_)
            {
                pthread_mutex_unlock(&Mutex_);
                return false;
            }
            Que_.push(Tt);
            pthread_mutex_unlock(&Mutex_);

            //放完数据的时候需要通知线程池当中的线程
            pthread_cond_signal(&Cond_);

            return true;
        }
        //取数据
        bool Pop(ThreadTask** Tt)
        {
            *Tt = Que_.front();
            Que_.pop();
            return true;
        }

        bool PoolClear()
        {
            pthread_mutex_lock(&Mutex_);
            IsQuit_ = true;
            pthread_mutex_unlock(&Mutex_);
            //当线程池当中的队列没有数据的时候，线程都是阻塞在pthread_cond_wait当中，等待被唤醒
            //while(ThreadCurNum_ > 0)
            if(ThreadCurNum_ > 0)
            {
                pthread_cond_broadcast(&Cond_);
            }
            return true;
        }
    private:
        //线程的入口函数
        static void* ThreadStart(void* arg)
        {
            ThreadPool* tp = (ThreadPool*)arg;
            while(1)
            {
                //运行A
                pthread_mutex_lock(&tp->Mutex_);
                //1 if(IsQuit_) 肯定不行的
               // if(tp->IsQuit_)
               // {
               //     tp->ThreadQuit();
               // }
                while(tp->Que_.empty())
                {
                    //2 if(IsQuit_)
                    if(tp->IsQuit_)
                    {
                        //如果不在while(tp->Que_empty())里面进行判断函数是否退出，则有可能在线程池队列没有数据的的时候，一直阻塞在pthread_cond_wait这个逻辑当中，及时被唤醒也是通过while循环判断之后又执行pthread_cond_wait
                        tp->ThreadQuit();
                    }
                    //运行B
                    //第一种：还在PCB等待队列当中
                    //第二种：已经被唤醒过一次，但是没有抢到锁资源，还在等待锁资源，
                    //        这种情况就可以归结成运行A的情况
                    pthread_cond_wait(&tp->Cond_, &tp->Mutex_);
                }
                //运行C
                ThreadTask* tt;
                tp->Pop(&tt);
                pthread_mutex_unlock(&tp->Mutex_);
                //由于已经拿到数据了，所以可以把锁释放掉了
                tt->Run();
                delete tt;
            }
            return NULL;
        }

        void ThreadQuit()
        {
            //临界
            ThreadCurNum_--;
            pthread_mutex_unlock(&Mutex_);
            pthread_exit(NULL);
        }
    private:
        bool ThreadCreate()
        {
            int ret = -1;
            pthread_t tid;
            for(int i = 0; i < THREADCOUNT; i++)
            {
                ret = pthread_create(&tid, NULL, ThreadStart, (void*)this);
                printf("[%d]\n", i);
                if(ret != 0)
                {
                    printf("create thread failed\n");
                    return false;
                }
            }
            return true;
        }
    private:
        //线程池当中 初始化的时候，线程的数量
        size_t ThreadCapacity_;
        //当前线程池有多少个数量
        size_t ThreadCurNum_;
        //线程安全的队列
        std::queue<ThreadTask*> Que_;
        //互斥
        pthread_mutex_t Mutex_;
        //同步机制
        pthread_cond_t Cond_;

        //线程退出的标志
        bool IsQuit_;
};

void Printf(int Data)
{
    printf("i am Handler func, i print [%d]\n", Data);
}

int main()
{
    ThreadPool* tp = new ThreadPool();
    for(int i = 0; i < 10; i++)
    {
        ThreadTask* tt = new ThreadTask(i, Printf);
        tp->Push(tt);
    }

    sleep(6);
    tp->PoolClear();
    delete tp;
    return 0;
}
