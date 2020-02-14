#include <pthread.h>
#include <unistd.h>
#include <iostream>

class MyThread
{
public:
    MyThread()
    {
        a = 60;
        sprintf(arr, "%s", "linux");
    }

    void print()
    {
        printf("%d-%s\n", a, arr);
    }

    static void* thread_run(void* arg)
    {
        MyThread* mythread = (MyThread*)arg;
        while(1)
        {
            sleep(1);
            mythread->print();
            printf("i am thread_run\n");
        }
        return NULL;
    }

    void CreateThread()
    {
        pthread_t tid;
        pthread_create(&tid, NULL, thread_run, this);
    }
private:
    int a;
    char arr[1024];
};

void* thread_start(void* arg)
{
    MyThread* mythread = (MyThread*)arg;
    while(1)
    {
        sleep(1);
        mythread->print();
        pthread_attr_t attr;
        size_t stacksize;
        pthread_attr_getstacksize(&attr, &stacksize);
        printf("%d\n", stacksize);
    }
    return NULL;
}

int main()
{
    MyThread* mt = new MyThread();
    mt->CreateThread();
    pthread_t tid;
    pthread_create(&tid, NULL, thread_start, mt);
    printf("%p\n", tid);

    while(1)
    {
        sleep(1);
        printf("i am main thread\n");
    }

    delete mt;
    return 0;
}

