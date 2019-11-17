#ifndef _COMM_H__
#define _COMM_H__

#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#define PATHNAME "."
#define PROJ_ID 0x6666
 
int CreateShm(int size);
int DestroyShm(int shmid);
int GetShm(int size);
#endif
