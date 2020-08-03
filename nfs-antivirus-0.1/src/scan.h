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
#ifndef _SCAN_H
#define _SCAN_H

#define ALL_SCAN 1
#define FAST_SCAN 2
#define CUSTOM_SCAN 3
#define SPECIFIED_SCAN 4
#define FIREFOX_DOWNLOAD_SCAN 5
#define CLICK_DIR_SCAN 6
#define CLICK_FILE_SCAN 7
#define RUN 1
#define STOP 0
#define MAX_PATH_SIZE 1024

#define DIRECTORY   1
#define REGULARFILE 2
#define OTHERTYPE 3

typedef struct node{
    char dirPath[MAX_PATH_SIZE];
    struct node *next;
    struct node *pre;
}dirNode;
unsigned long countFilesNumber(char *path);
void printprocdir(char *procdir, char *expectTime);
void scan_procdir();
void scan_specified_dir(char *scanPath,long int pathSize,int scantype);
void listdir(char *path, char *expectTime,int scantype);
void quitScanThread();
void allScanThread();
void specifiedScanThread(char *scanPath);
int get_scan_size(int scanType);
int fileNumberToTime(char *mode, char expectTime[]);
int getThreadNum();
int pathTransformSpecialSymbols(char *path, char outPath[]);
int isLinkFile(char *filePath, char newFilePath[]);
void accord_type_scan_file(int *scanType);
int is_inWhiteList(char *filePath);
void get_expect_time_thread();
#endif
