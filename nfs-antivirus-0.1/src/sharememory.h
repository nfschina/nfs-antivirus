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
struct scanResults{
        char filename[256];
        int scanStatus;
        int scanObjNum;
        int scanThreatNum;
        char scanExpectTime[30];
        int filesize;
        int pathsSize;
};

struct DeleteVirus{
float total_select_num;
float cur_delete_num;
};
int write_share_memory(char *semkeyName ,char *shmkeyName,struct scanResults *scanResultInfo);
int read_share_memory(char *semkeyName ,char *shmkeyName,struct scanResults *scanResultInfo);
int write_share_memory_delete_info(char *semkeyName ,char *shmkeyName, struct DeleteVirus *deleteVirusInfo);
int read_share_memory_delete_info(char *semkeyName ,char *shmkeyName,struct DeleteVirus *deleteVirusInfo);
void p( int * semid);
void v( int * semid);
