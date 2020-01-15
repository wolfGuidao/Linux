#pragma once
#include<iostream>
#include<stdio.h>
#include"blockingqueue.hpp"
using namespace std;

class Task
{
public:
    virtual void Run()
    {

    }
    virtual ~ Task()
    {

    }
};

//线程池对象启动时创建一组线程
//每个线程都要完成一些任务，即执行一定代码逻辑这个逻辑由调用者决定
class ThreadPool
{
public:
    //n表示要创建线程的个数
    ThreadPool(int n)
    :queue_(100)
    ,worker_count_(n)
    {
        //创建线程
        for(int i=0;i<worker_count_;i++)
        {
            pthread_t tid;
            pthread_create(&tid,NULL,ThreadEntry,this);
            workers_.push_back(tid);
        }
    }

   ~ThreadPool()
    {
        //先让线程结束，然后才回收
        for(size_t i=0;i<workers_.size();i++)
        {
            pthread_cancel(workers_[i]);
        }
        for(size_t i=0;i<workers_.size();i++)
        {
            pthread_join(workers_[i],NULL);
        }
    }
    //使用线程池的时候，就需要调用者加入一些任务，让线程池去执行
    void AddTask(Task* task)
    {
        queue_.Push(task);
    }

    static void* ThreadEntry(void* arg)
    {
        ThreadPool* pool=(ThreadPool*)arg;
        while(true)
        {
            //循环中尝试从阻塞队列中获取一个任务
            //然后去执行
            Task* task=NULL;
            pool->queue_.Pop(&task);
            //task表面上是一个Task*类型，实际指向的是MyTask*类型
            task->Run();
            delete task;
        }
        return NULL;

    }
private:
Blockingqueue<Task*>queue_;
int worker_count_;
vector<pthread_t>workers_;
};
