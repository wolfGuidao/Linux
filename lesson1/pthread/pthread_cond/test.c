#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>

#define THREAD_NUM 1

//创建互斥锁或互斥量==》控制互斥的
pthread_mutex_t mutex;
//条件变量==》控制同步的
pthread_cond_t cond;

void* ThreadEntry1(void* arg)
{
    (void)arg;
    while(1)
    {
        printf("传球\n");
        //pthread_cond_wait(&cond,&mutex)的等待条件
        pthread_cond_signal(&cond);
        sleep(4);
    }
    return NULL;
}
void* ThreadEntry2(void* arg)
{
    (void)arg;
    while(1)
    {
        //pthread_cond_wait(&cond,&mutex)一旦被执行，就会导致线程被阻塞，直到其他线程发送等待条件结束的通知
        //即pthread_cond_signal(&cond)发送的通知
        pthread_cond_wait(&cond,&mutex);
        printf("扣篮\n");
        sleep(1);
    }
    return NULL;
}
int main()
{
    //对互斥锁进行初始化
    pthread_mutex_init(&mutex,NULL);
    //对条件变量进行初始化
    pthread_cond_init(&cond,NULL);

    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,ThreadEntry1,NULL);
    pthread_create(&tid2,NULL,ThreadEntry2,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    //释放互斥锁
    pthread_mutex_destroy(&mutex);
    //对条件变量进行释放
    pthread_cond_destroy(&cond);
    return 0;
}
