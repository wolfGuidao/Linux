#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>

#define shm_key 0x12345678
int main()
{
    //创建共享内存
    //shmget
    int shmid = shmget(shm_key, 1024, IPC_CREAT | 0664);
    if(shmid < 0)
    {
        return 0;
    }
    printf("%d\n", shmid);

    struct shmid_ds buf;
    shmctl(shmid, IPC_STAT, &buf);
    printf("shm_size : %ld\n", buf.shm_segsz);

    void *lp = shmat(shmid, NULL, 0);
    if(!lp)
    {
        return 0;
    }

    while(1)
    {
        sprintf((char*)lp, "%s", "do you eat\n");
        sleep(1);
    }

    shmdt(lp);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
