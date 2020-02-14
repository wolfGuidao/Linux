#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define THREADCOUNT 4
//对于全局变量g_noodle的取值，我们为了验证线程之间进行同步，
//我们规定 0 是没有面，1 是有面的情况
int g_noodle = 0;

//定义互斥锁资源
pthread_mutex_t g_mutex;
//吃饭的条件变量
pthread_cond_t g_cond;
//做饭的条件变量
pthread_cond_t g_makecond;

void* EatStart(void* arg)
{
    (void)arg;
    while(1)
    {
        pthread_mutex_lock(&g_mutex);
        while(g_noodle == 0)
        {
            pthread_cond_wait(&g_cond, &g_mutex);
        }
        g_noodle--;
        printf("i am [%p], eat one noodle:[%d]\n", pthread_self(), g_noodle);
        pthread_mutex_unlock(&g_mutex);
        pthread_cond_signal(&g_makecond);
    }
    return NULL;
}

void* MakeStart(void* arg)
{
    (void)arg;
    while(1)
    {
        //互斥锁只是保护临界资源的
        pthread_mutex_lock(&g_mutex);
        while(g_noodle == 1)
        {
            pthread_cond_wait(&g_makecond, &g_mutex);
        }
        g_noodle++;
        printf("i am [%p], i make one noodle:[%d]\n", pthread_self(), g_noodle);
        pthread_mutex_unlock(&g_mutex);
        pthread_cond_signal(&g_cond);
    }
    return NULL;
}


int main()
{
    pthread_mutex_init(&g_mutex, NULL);
    pthread_cond_init(&g_cond, NULL);
    pthread_cond_init(&g_makecond, NULL);
    pthread_t eat_tid[THREADCOUNT], make_tid[THREADCOUNT];
    int i = 0;
    int ret = -1;
    for(; i < THREADCOUNT; i++)
    {
        ret = pthread_create(&eat_tid[i], NULL, EatStart, NULL);
        if(ret != 0)
        {
            printf("create thread failed\n");
            return 0;
        }

        ret = pthread_create(&make_tid[i], NULL, MakeStart, NULL);
        if(ret != 0)
        {
            printf("create thread failed\n");
            return 0;
        }
    }

    for(i = 0; i < THREADCOUNT; i++)
    {
        pthread_join(eat_tid[i], NULL);
        pthread_join(make_tid[i], NULL);
    }
    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_cond);
    pthread_cond_destroy(&g_makecond);
    return 0;
}
