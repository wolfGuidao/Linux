#pragma once

#include<iostream>
#include<vector>
#include<semaphore.h>
using namespace std;
//规定Blockingqueue的长度是有上限的
//如果队列为空或者满了，去执行pop或push都会阻塞

//实现同步
//信号量表示可用资源的个数
//一个信号量表示当前队列中元素的个数
//另一个信号量表示当前队列中空格的个数
//即元素和空格之间是互斥的，插入一个资源，就消耗一个空格资源，相当于释放了一个元素资源
//删除一个元素，即相当于消耗一个元素资源，相当于释放了一个空格资源
template<typename T>
class Blockingqueue
{
public:
    Blockingqueue(int max_size)
        :max_size_(max_size)
         ,head_(0)
         ,tail_(0)
         ,size_(0)
         ,queue_(max_size)

    {
        sem_init(&lock_,0,1);
        sem_init(&elem_,0,0);
        sem_init(&blank_,0,max_size);
    }

    ~Blockingqueue()
    {
        sem_destroy(&lock_);
        sem_destroy(&elem_);
        sem_destroy(&blank_);
    }
    void Push(const T& data)
    {
        //每次插入元素，就得先申请一个空格资源
        //如果没有空格资源（即信号量为0），就说明队列满了
        //满了就不能直接继插入，需要在Push中阻塞
        sem_wait(&blank_);


        //注意信号量的+1 -1是原子的，
        sem_wait(&lock_);
        queue_[tail_]=data;
        ++tail_;
        ++size_;
        sem_post(&lock_);

        //如果插入成功说明元素个数多一个，就释放元素资源
        sem_post(&elem_);
    }

    //用输出型参数data表示出队列的元素
    void Pop(T* data)
    {
        //每次出队列元素，就得申请一个元素资源
        //如果队列中没有元素资源，即队列为空
        //就不能直接出队列，需要在Pop出阻塞
        sem_wait(&elem_);

        sem_wait(&lock_);
        *data = queue_[head_];
        ++head_;
        --size_;
        sem_post(&lock_);

        //释放一个空格资源
        sem_post(&blank_);
    }
private:
    vector<T> queue_;
    int head_;
    int tail_;
    int size_;
    int max_size_;
    //用二元信号量lock_表示互斥锁,即资源个数非0即1
    sem_t lock_;
    //表示可用元素的个数
    sem_t elem_;
    //表示可用空格的个数
    sem_t blank_;
};
