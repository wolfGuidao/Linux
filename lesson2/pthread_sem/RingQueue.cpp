#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

using namespace  std;
#define NUM 10

class RingQueue
{
  public:
    RingQueue(int _cap = NUM)
      :q(_cap)
       ,cap(_cap)
  {
    sem_init(&data_sem,0,0);
    sem_init(&space_sem,0,cap);
    consume_step = 0;
    product_step = 0;

  }

    //新增一个数据就相当于就减少一个空格资源,增加一个数据资源
    void PutData(const int& data)
    {
      sem_wait(&space_sem);

      q[consume_step] = data;
      consume_step++;
      consume_step %= cap;

      sem_post(&data_sem);
    }

    //取出一个资源相当于释放一个数据资源，增加一个空格资源
    void GetDate(int& data)
    {
      sem_wait(&data_sem);

      data = q[product_step];
      product_step++;
      product_step %= cap;

      sem_post(&space_sem);
    }

   ~RingQueue()
   {
     sem_destroy(&data_sem);
     sem_destroy(&space_sem);
   }


  private:
    vector<int> q;
    int cap;
    sem_t data_sem;
    sem_t space_sem;
    int consume_step;
    int product_step;

};

void* consumer(void* arg)
{
  RingQueue* rap = (RingQueue*)arg;
  int data ;
  while(1)
  {
    rap->GetDate(data);
    cout<<"consumer data done: "<<data<<endl;
    sleep(3);
  }
}

void* productr(void* arg)
{
  RingQueue* rap = (RingQueue*)arg;
  srand((unsigned long)time(NULL));
  while(1)
  {
    //int data = rand() % 1024;
    int array[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
    for(auto e : array)
    {
      int data = e;
    rap->PutData(data);
    cout<<"producter data done: "<<data<<endl;
    }
    sleep(3);
  }
}

int main()
{
  RingQueue rq;
  pthread_t c,p;
  pthread_create(&c,NULL,consumer,(void*)&rq);
  pthread_create(&p,NULL,productr,(void*)&rq);

  pthread_join(c,NULL);
  pthread_join(p,NULL);

  return 0;
}
