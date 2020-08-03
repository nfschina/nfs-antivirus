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
#include <gtk/gtk.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include "upgradeself.h"
#include "../function.h"
#include "../page/upgrade-page.h"

/***dsj-update****/
/**
  *  功能:      比对版本是否为最新     
  *  输入参数:  @param get_new_version_cmd 为获取新版本命令行
                @param get_old_version_cmd 为获取旧版本命令行
  *  返回值:    若为最新返回值为0，否则为1
**/

int compare_new_old_version(char *get_new_version_cmd,char *get_old_version_cmd)
{
//      if (new_version==NULL || old_version==NULL)
//      {
//              return -1;
//      }
        char new_version_temp[512];
        char old_version_temp[512];
        cmd_shell_result(get_new_version_cmd,new_version_temp,sizeof(new_version_temp));
        cmd_shell_result(get_old_version_cmd,old_version_temp,sizeof(old_version_temp));
        if (strcmp(new_version_temp,old_version_temp)==0)
        {
                return 0;
        }
        else
        {
                return 1;
        }
}

/***dsj-update****/

int get_antivirus_status()
{
    FILE *pstatusFile;
    char buffer[30];
    char *pbuf;
    char retBuffer[30];
    char *pretBuf =NULL;
    int iAvlStatus = 0;
    char avl_status_file[100],upgrade_status_file[100];
    char szAvlStatus[10];
    int avlStatus; 

    snprintf(avl_status_file,100,"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    snprintf(upgrade_status_file,100,"%savlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR);

    avlStatus = get_upgrde_status(upgrade_status_file);

    if((pstatusFile = fopen(avl_status_file,"r+")) == NULL)
    {
        return -1;
    }
    pbuf = buffer ;
    fgets(pbuf,30,pstatusFile);
    fclose(pstatusFile);
    if(pbuf != NULL)
    {
        iAvlStatus = atoi(pbuf);
    } 
    if(iAvlStatus == AVL_INIT_STATUS || iAvlStatus == AVL_SCAN_STATUS ||  avlStatus == AVL_UPGADE_STATUS)
    {
        return AVL_IS_USE_LIB;
    }
    else
    {
        if((pstatusFile = fopen(upgrade_status_file,"w+")) == NULL)
        {
            return -1;
        }
        strncpy(szAvlStatus,"6",sizeof(szAvlStatus));
        fwrite(szAvlStatus,strlen(szAvlStatus),1,pstatusFile);
        fclose(pstatusFile);
    }
    return AVL_IS_NOT_USE_LIB;
}
void write_antivirus_status(char *filename,char *szStatus)
{
    FILE *pstatusFile;
    char szAvlStatus[10];

    if((pstatusFile = fopen(filename,"w+")) == NULL)
    {
        return ;
    }
    strncpy(szAvlStatus,szStatus,sizeof(szAvlStatus));
    fwrite(szAvlStatus,strlen(szAvlStatus),1,pstatusFile);
    fclose(pstatusFile);
}
int startUpgrade()
{
    printf("^^^^^^^^准备开始升级^^^^^^^^^\n");
    char upgradeCmd[256] = {0};
    char getVersionCmd[256] = {0};
    char oldVersion[256] = {0};
    char newVersion[256] = {0};
    char upgrade_status_file[100];

    FILE * upgradeId;

    snprintf(upgrade_status_file,100,"%savlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR);
    printf("一、开始网络检测");

/**网络检测********/
    if (Check_Net_Connect() == -1)
    {
        printf("1.1 检测结果为网络连接失败。\n");
        write_antivirus_status(upgrade_status_file,"7");
        return AVL_UPGADE_FAIL;

    }
    else
    {
        write_antivirus_status(upgrade_status_file,"6");
        printf("1.1 检测结果为网络连接成功。\n");
	return AVL_UPGADE_SUCESS;
    }

	
}
