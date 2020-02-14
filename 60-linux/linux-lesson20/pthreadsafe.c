#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
//黄牛抢票程序
//4个黄牛，100张票
//黄牛A, 黄牛B, 黄牛C, 黄牛D
//1        2      3     4
//5       6       7     8
//黄牛：线程来代替
//100票：全局变量来表示

//g_tickets是临界资源
int g_tickets = 100;
#define THREADCOUNT 4
//定义互斥锁变量
pthread_mutex_t g_mutex;

typedef struct ThreadPara
{
    int ThreadId_;
    //... and so on
}TP;

void* ThreadStart(void* arg)
{
    TP* tp = (TP*)arg;
    while(1)
    {
        //2
        pthread_mutex_lock(&g_mutex);
        if(g_tickets > 0)
        {
            usleep(1000);
            printf("i am thread[%d], i am ticket is [%d]\n", tp->ThreadId_, g_tickets);
            g_tickets--;
        }
        else
        {
            pthread_mutex_unlock(&g_mutex);
            delete tp;
            tp = NULL;
            break;
        }
        pthread_mutex_unlock(&g_mutex);
    }
    return NULL;
}

int main()
{
    //线程创建之前来初始化互斥锁变量
    pthread_mutex_init(&g_mutex, NULL);
    pthread_t tid[THREADCOUNT];
    int i = 0;
    for(; i < THREADCOUNT; i++)
    {
        TP* tp = new TP;
        tp->ThreadId_ = i;
        pthread_create(&tid[i], NULL, ThreadStart, (void*)tp);
    }

    for(i = 0; i < THREADCOUNT; i++)
    {
        pthread_join(tid[i], NULL);
    }
    //销毁互斥锁变量
    pthread_mutex_destroy(&g_mutex);
    return 0;
}
