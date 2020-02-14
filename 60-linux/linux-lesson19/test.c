#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void* thread_start(void* arg)
{
    pthread_detach(pthread_self());
    printf("i am thread_start\n");
    sleep(5);
    char* arr = (char*)malloc(10);
    memset(arr, '0', 10);
    strcpy(arr, "linux-60");
    pthread_exit((void*)arr);
    sleep(1);
    while(1)
    {
        sleep(1);
        printf("i am thread_start:%s\n", (char*)arg);
    }
    return NULL;
}

pthread_t CreateThread()
{
    pthread_t tid;
    char arr[1024] = {0};
    sprintf(arr, "%s", "linux-60");
    char* lp = "nihao~";
    pthread_create(&tid, NULL, thread_start, (void*)lp);
    return tid;
}

int main()
{
    //create thread
    pthread_t tid = CreateThread();
    //pthread_detach(tid);

    printf("main thread exit\n");
    sleep(2);
    //pthread_exit(NULL);
    //void * retval;
    //pthread_join(tid, &retval);
    //printf("retval:%s\n", (char*)retval);
    while(1)
    {
        sleep(1);
        printf("i am main thread\n");
    }
    return 0;
}
