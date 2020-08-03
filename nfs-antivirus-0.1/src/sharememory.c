/**
* Copyright (c) 2016-2018 CPU and Fundamental Software Research Center, CAS
*
* This software is published by the license of CPU-OS License, you can use and
* distibuted this software under this License. See CPU-OS Liense for more detail.
*
* You shoud have receive a copy of CPU-OS License. If not, please contact us 
* by email <support_os@cpu-os.ac.cn>
*
**/
#include <stdio.h>    
#include <sys/types.h>    
#include <sys/ipc.h>    
#include <sys/sem.h>    
#include <string.h>
#include "sharememory.h"
#include <sys/shm.h>

/*信号量的P操作*/
void p( int * semid)
{
    struct sembuf sem_p;
    sem_p.sem_num=0;
    sem_p.sem_op=-1;
    if(semop(*semid,&sem_p,1)==-1)
        printf("p operation is fail/n");
}

/*信号量的V操作*/
void v(int * semid)
{
    struct sembuf sem_v;
    sem_v.sem_num=0;
    sem_v.sem_op=1;
    if(semop(*semid,&sem_v,1)==-1)
        printf("v operation is fail/n");
}

int write_share_memory(char *semkeyName ,char *shmkeyName, struct scanResults *scanResultInfo)  
{    
        
    int semid;    
    int shmid;    
    key_t semkey;    
    key_t shmkey;    
    semkey=ftok(semkeyName,0);    
    shmkey=ftok(shmkeyName,0);    
        
    /*创建共享内存和信号量的IPC*/    
    semid=semget(semkey,1,0666|IPC_CREAT);    
    if(semid==-1)
    {    
        return -1;
    }
    shmid=shmget(shmkey,1024,0666|IPC_CREAT);    
    if(shmid==-1)    
    {
        return -1;
    }    
    /*设置信号量的初始值，就是资源个数*/    
    union semun{    
        int val;    
        struct semid_ds *buf;    
        ushort *array;    
    }sem_u;    
        
    sem_u.val=1;    
    semctl(semid,0,SETVAL,sem_u);    
        
    /*将共享内存映射到当前进程的地址中，之后直接对进程中的地址addr操作就是对共享内存操作*/    
        
    struct scanResults * addr;    
    addr=(struct scanResults*)shmat(shmid,0,0);    
    if(addr==(struct scanResults*)-1)    
    {
        return -1;
    }    
    /*向共享内存写入数据*/    
    p(&semid);    
    strcpy((*addr).filename,scanResultInfo->filename);    
    (*addr).scanStatus = scanResultInfo->scanStatus;
    (*addr).scanObjNum = scanResultInfo->scanObjNum;
    (*addr).scanThreatNum = scanResultInfo->scanThreatNum;
    strcpy((*addr).scanExpectTime,scanResultInfo->scanExpectTime);
    (*addr).filesize = scanResultInfo->filesize;    
    (*addr).pathsSize = scanResultInfo->pathsSize;
    v(&semid);    
        
    /*将共享内存与当前进程断开*/    
    if(shmdt(addr)==-1)    
    {
        return -1 ;
    }    
}    
int read_share_memory(char *semkeyName ,char *shmkeyName,struct scanResults *scanResultInfo)
{
    int semid;
    int shmid;
    key_t semkey;
    key_t shmkey;
    semkey=ftok(semkeyName,0);
    shmkey=ftok(shmkeyName,0);


    /*读取共享内存和信号量的IPC*/
    semid=semget(semkey,0,0666);
    if(semid==-1)
    {
        return -1;
    }
    shmid=shmget(shmkey,0,0666);
    if(shmid==-1)
    printf("creat shm is fail\n");
        /*将共享内存映射到当前进程的地址中，之后直接对进程中的地址addr操作就是对共享内存操作*/
    struct scanResults *scanMemoryInfo;
    scanMemoryInfo=(struct scanResults*)shmat(shmid,0,0);
    if(scanMemoryInfo==(struct scanResults*)-1)
    {
        return -1;
    }
   /*从共享内存读出数据*/
    p(&semid);
    strcpy(scanResultInfo->filename,scanMemoryInfo->filename);
    scanResultInfo->scanStatus = scanMemoryInfo->scanStatus;
    scanResultInfo->scanObjNum = scanMemoryInfo->scanObjNum;
    scanResultInfo->scanThreatNum = scanMemoryInfo->scanThreatNum;
    strcpy(scanResultInfo->scanExpectTime,scanMemoryInfo->scanExpectTime);
    scanResultInfo->filesize = scanMemoryInfo->filesize;
    scanResultInfo->pathsSize = scanMemoryInfo->pathsSize;
    v(&semid);

    /*将共享内存与当前进程断开*/
    if(shmdt(scanMemoryInfo)==-1)
    {
        return -1;
    }
    
    /*IPC必须显示删除。否则会一直留存在系统中*/
    if(semctl(semid,0,IPC_RMID,0)==-1)
    {
        return -1;
    }
    if(shmctl(shmid,IPC_RMID,NULL)==-1)
    {
        return -1;
    }

    return 1;
}



int read_share_memory_delete_info(char *semkeyName ,char *shmkeyName,struct DeleteVirus *deleteVirusInfo)
{
    int semid;
    int shmid;
    key_t semkey;
    key_t shmkey;
    semkey=ftok(semkeyName,0);
    shmkey=ftok(shmkeyName,0);


    /*读取共享内存和信号量的IPC*/
    semid=semget(semkey,0,0666);
    if(semid==-1)
    {
        return -1;
    }
    shmid=shmget(shmkey,0,0666);
    if(shmid==-1)
    printf("creat shm is fail\n");
        /*将共享内存映射到当前进程的地址中，之后直接对进程中的地址addr操作就是对共享内存操作*/
    struct DeleteVirus  *deleteVirusMemoryInfo;
    deleteVirusMemoryInfo =(struct DeleteVirus*)shmat(shmid,0,0);
    if(deleteVirusMemoryInfo ==(struct DeleteVirus*)-1)
    {
        return -1;
    }
   /*从共享内存读出数据*/
    p(&semid);
    deleteVirusInfo->total_select_num = deleteVirusMemoryInfo->total_select_num;
    deleteVirusInfo->cur_delete_num = deleteVirusMemoryInfo->cur_delete_num;
    v(&semid);

    /*将共享内存与当前进程断开*/
    if(shmdt(deleteVirusMemoryInfo)==-1)
    {
        return -1;
    }

    /*IPC必须显示删除。否则会一直留存在系统中*/
    if(semctl(semid,0,IPC_RMID,0)==-1)
    {
        return -1;
    }
    if(shmctl(shmid,IPC_RMID,NULL)==-1)
    {
        return -1;
    }

    return 1;
}



int write_share_memory_delete_info(char *semkeyName ,char *shmkeyName, struct DeleteVirus *deleteVirusInfo)
{

    int semid;
    int shmid;
    key_t semkey;
    key_t shmkey;
    semkey=ftok(semkeyName,0);
    shmkey=ftok(shmkeyName,0);

    /*创建共享内存和信号量的IPC*/
    semid=semget(semkey,1,0666|IPC_CREAT);
    if(semid==-1)
    {
        return -1;
    }
    shmid=shmget(shmkey,1024,0666|IPC_CREAT);
    if(shmid==-1)
    {
        return -1;
    }
    /*设置信号量的初始值，就是资源个数*/
    union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
    }sem_u;

    sem_u.val=1;
    semctl(semid,0,SETVAL,sem_u);

    /*将共享内存映射到当前进程的地址中，之后直接对进程中的地址addr操作就是对共享内存操作*/

    struct DeleteVirus  * addr;
    addr=(struct DeleteVirus *)shmat(shmid,0,0);
    if(addr==(struct DeleteVirus *)-1)
    {
        return -1;
    }
    /*向共享内存写入数据*/
    p(&semid);
    (*addr).total_select_num = deleteVirusInfo->total_select_num;
    (*addr).cur_delete_num = deleteVirusInfo->cur_delete_num;
    v(&semid);

    /*将共享内存与当前进程断开*/
    if(shmdt(addr)==-1)
    {
        return -1 ;
    }
}

