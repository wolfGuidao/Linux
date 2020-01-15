#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <queue>
//using namespace std;

#define NUM 8

class BlockQueue
{
  private:
    void LockQueue()
    {
      pthread_mutex_lock(&lock);
    }

    void UnLockQueue()
    {
      pthread_mutex_unlock(&lock);
    }

    void ProductWait()
    {
      pthread_cond_wait(&full,&lock);
    }

    void ConsumeWait()
    {
      pthread_cond_wait(&empty,&lock);
    }

    void NoitfyProduct()
    {
      pthread_cond_signal(&full);
    }

    void NoitfyConsume()
    {
      pthread_cond_signal(&empty);
    }

    bool IsEmpty()
    {
      return (q.size() == 0 ? true : false);
    }

    bool IsFull()
    {
      return (q.size() == cap ? true : false);
    }

  public:
    BlockQueue(size_t _cap = NUM)
      :cap(_cap)
    {
      pthread_mutex_init(&lock,NULL);
      pthread_cond_init(&full,NULL);
      pthread_cond_init(&empty,NULL);
    }

    void PushDate(const int& data)
    {
      LockQueue();
      while(IsFull())
      {
        NoitfyConsume();
        std::cout<<"queue is full,notify consume data,product stop"<<std::endl;
        ProductWait();
      }
      q.push(data);
      NoitfyConsume();
      UnLockQueue();
    }

    void PopData(int& data)
    {
      LockQueue();
      while(IsEmpty())
      {
        NoitfyProduct();
        std::cout<<"queue is empty,NoitfyProduct data,consume stop."<<std::endl;
        ConsumeWait();
      }
      data = q.front();
      q.pop();
      NoitfyProduct();
      UnLockQueue();
    }

    ~BlockQueue()
    {
      pthread_mutex_destroy(&lock);
      pthread_cond_destroy(&full);
      pthread_cond_destroy(&empty);
    }

  private:
    std::queue<int>q;
    //int cap;
    size_t cap;
    pthread_mutex_t lock;
    pthread_cond_t full;
    pthread_cond_t empty;
};

void* Consumer(void* arg)
{
  BlockQueue* bqp = (BlockQueue*)arg;
  int data;
  while(1)
  {
    bqp->PopData(data);
    std::cout<<"Consume data done :"<<data<<std::endl;
    sleep(3);
  }
}

void* Product(void* arg)
{
  BlockQueue* bqp = (BlockQueue*)arg;
  srand((unsigned long)time(NULL));
  while(1)
  {
    int data = rand() % 1024;
    //int array[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    //int data;
    //for(auto e : array)
    //{
    //  data = e;
    
    bqp->PushDate(data);
    std::cout<<"Product data done!"<<std::endl;
    sleep(1);

  }
}

int main()
{
  BlockQueue bq;
  pthread_t c,p;

  pthread_create(&c,NULL,Consumer,(void*)&bq);
  pthread_create(&p,NULL,Product,(void*)&bq);

  pthread_join(c,NULL);
  pthread_join(p,NULL);

  return 0;
}
