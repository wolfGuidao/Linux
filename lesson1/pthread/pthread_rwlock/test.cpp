#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
using namespace std;

pthread_rwlock_t lock;

//1.交易场所
int g_count=0;

//2.两种角色
void* Reader(void* arg)
{
    (void)arg;
    while(1)
    {
        //获取读锁
        pthread_rwlock_rdlock(&lock);

        printf("count = %d\n",g_count);

        pthread_rwlock_unlock(&lock);
        sleep(1);
    }
    return NULL;
}
void* Writer(void* arg)
{
    (void)arg;
    int count=0;
    while(1)
    {
        //获取写锁
        pthread_rwlock_wrlock(&lock);

        ++count;
        g_count=count;

        pthread_rwlock_unlock(&lock);
        sleep(3);
    }
    return NULL;
}

int main()
{
   pthread_rwlock_init(&lock,NULL);

    pthread_t tid1,tid2,tid3,tid4;
    pthread_create(&tid1,NULL,Reader,NULL);
    pthread_create(&tid3,NULL,Reader,NULL);
    pthread_create(&tid4,NULL,Reader,NULL);
    pthread_create(&tid2,NULL,Writer,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);
    pthread_join(tid2,NULL);

    pthread_rwlock_destroy(&lock);
    return 0;

}
