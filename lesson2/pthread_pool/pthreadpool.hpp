#pragma once
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <stdio.h>

#define MAX_THREAD 4
#define MAX_QUEUE  10
using namespace std;

class MyTask
{
  public:
    MyTask()
    {

    }

    ~MyTask()
    {

    }

    void SetData(int _data)
    {
      data = _data;
    }

    void Run()
    {
      int t = rand() % 5;
      printf("thread: %lu sleep %d sec,execute data:%d\n",pthread_self(),t,data);
      sleep(t);
    }

  private:
    int data;
};

class MyThreadPool
{
  
  private:
    void ThreadLock()
    {
      pthread_mutex_lock(&mutex);
    }

    void ThreadUnLock()
    {
      pthread_mutex_unlock(&mutex);
    }

    void ConsumerWait()
    {
      pthread_cond_wait(&empty,&mutex);
    }

    void ConsumerNotify()
    {
      pthread_cond_signal(&empty);
    }

    void ProducterWait()
    {
      pthread_cond_wait(&full,&mutex);

    }

    void ProducterNotify()
    {
      pthread_cond_signal(&full);
    }

    bool ThreadIsRunning()
    {
      return (keep_running == 0 ? false : true);
    }

    void ThreadExit()
    {
      cur_thread--;
      printf("thread : %lu exit\n",pthread_self());
      ProducterNotify();
      pthread_exit(NULL);
    }

    bool QueueIsEmpty()
    {
      return (task_list.size() == 0 ? true : false);
    }

    bool QueueIsFull()
    {
      return (task_list.size() == max_queue ? true : false);

    }

    void PopTask(MyTask** task)
    {
      *task = task_list.front();
      task_list.pop();

    }

    void PushTask(MyTask* task)
    {
      task_list.push(task);
    }

    static void* thread_routine(void* arg)
    {
      MyThreadPool* pthreadpool = (MyThreadPool*)arg;
      while(1)
      {
        pthreadpool->ThreadLock();

        //若当前是与运行状态，并且没有任何任务挂起等待
        while(pthreadpool->QueueIsEmpty() && pthreadpool->ThreadIsRunning())
        {
          pthreadpool->ConsumerWait();

        }

        //若当前是非运行状态，且没有任务退出线程
        //若当前是非运行状态，但是队列中有任务则需要将任务执行完毕后才能退出
        if(!pthreadpool->ThreadIsRunning() && pthreadpool->QueueIsEmpty())
        {
          pthreadpool->ThreadUnLock();
          pthreadpool->ThreadExit();
        }

        //走到这里说明任务队列不为空，那么获取一个任务去执行
        MyTask* task;
        pthreadpool->PopTask(&task);
        pthreadpool->ProducterNotify();
        pthreadpool->ThreadUnLock();

        //执行任务不能在锁内，否则会导致其他线程饥饿
        task->Run();
      }
      return NULL;
    }

  public:
    MyThreadPool(int _max_thread = MAX_THREAD,int _max_queue = MAX_QUEUE)
      :max_thread(_max_thread)
       ,cur_thread(_max_thread)
       ,keep_running(1)
       ,max_queue(_max_queue)
  {
    int i = 0;

    printf("create thread:%d - %d\n",max_thread,max_queue);
    tid = (pthread_t*)malloc(sizeof(pthread_t) * _max_thread);

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&empty,NULL);
    pthread_cond_init(&full,NULL);


    //创建指定数量的线程，等待任务执行
    for(int i = 0;i < _max_thread;i++)
    {
      int ret = pthread_create(&tid[i],NULL,thread_routine,(void*)this);
      if(ret != 0)
      {
        printf("create thread is error!\n");
        exit(0);
      }

      pthread_detach(tid[i]);
    }
  }

    ~MyThreadPool()
    {
      pthread_mutex_destroy(&mutex);
      pthread_cond_destroy(&empty);
      pthread_cond_destroy(&full);
    }

    //向线程池添加任务队列
    bool AddTaskToPool(MyTask* task)
    {
      ThreadLock();
      while(QueueIsFull())
      {
        ProducterWait();
      }
      PushTask(task);
      printf("add task to pool\n");
      ConsumerNotify();
      ThreadUnLock();
      return true;
    }

    void StopThreadPool()
    {
      //若已经调用过线程池退出，则返回0
      if(keep_running == 0)
      {
        return ;
      }
      ThreadLock();
      keep_running = 0;
      ///如果还有线程没有退出，则挂起等待
      //等到所有线程将队列中的所有任务都执行完毕后并且退出
      while(cur_thread > 0)
      {
        ProducterWait();
      }
      ThreadUnLock();
    };


  private:
    int max_thread;//线程池中最大的线程数
    int cur_thread;//当前线程池中的线程数
    int keep_running;//用于停止线程池中的线程标志
    int max_queue;//队列中最大节点数
    pthread_t* tid;//线程id
    queue<MyTask*> task_list;
    pthread_mutex_t mutex;
    pthread_cond_t empty;
    pthread_cond_t full;
};
