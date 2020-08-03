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
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include "writelog.h"
#include "function.h"
int write_scan_log(char *logFileName ,struct scanLogInfo * one_scanLogInfo)
{
    	FILE *pLogFile,*pLogFile_deal;
    	char *pLogBuf,*pLogBuf_deal;
	char outPath[1024]={0};
	char file_temp[1024];
	snprintf(file_temp,sizeof(file_temp),"%sscan_result_deal.txt",ANTIVIRUS_TEMP_DIR);
    	if((pLogFile = fopen(logFileName,"ab")) == NULL)
    	{
        	return -1;
    	}
    	if((pLogFile_deal = fopen(file_temp,"ab")) == NULL)
    	{
        	return -1;
    	}

    	pLogBuf = (char *) malloc(sizeof(struct scanLogInfo) +100);
    	pLogBuf_deal = (char *) malloc(sizeof(struct scanLogInfo) +100);

    	pathTransformSpecialSymbols(one_scanLogInfo->scanFileName, outPath);

    	sprintf(pLogBuf,"%s;%s;%d;%s;\n",one_scanLogInfo->starttime,one_scanLogInfo->scanFileName,one_scanLogInfo->threatNum,one_scanLogInfo->threatName);

    	fwrite(pLogBuf,strlen(pLogBuf),1,pLogFile);

    	free(pLogBuf);
    	fclose(pLogFile);

    	sprintf(pLogBuf_deal,"%s;%s;%d;%s;\n",one_scanLogInfo->starttime,outPath,one_scanLogInfo->threatNum,one_scanLogInfo->threatName);

    	fwrite(pLogBuf_deal,strlen(pLogBuf_deal),1,pLogFile_deal);

    	free(pLogBuf_deal);
    	fclose(pLogFile_deal);
}


