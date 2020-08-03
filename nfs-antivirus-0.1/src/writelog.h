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
struct scanLogInfo
{
    char starttime[30];
    char endtime[30];
    char scanFileName[2048];
    int threatNum ;
    char usetime[30];
    char threatName[200];
};
int write_scan_log(char *logFileName ,struct scanLogInfo * one_scanLogInfo);

