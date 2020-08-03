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
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <libintl.h>
#include "upgrade.h"
#define _(STRING) gettext(STRING)
//extern GtkWidget *last_update_label;
//extern char scan_starttime[30];
struct SDate
{
        short int year;
        short int month;
        short int day;
};
int IsLeapYear(short year)
{
    return year%4==0&&year%100||year%400==0;
}
int DaysOfOneYear(struct SDate sd)
{
    switch(sd.month-1)
    {
        case 11:
           sd.day+=30;
        case 10:
           sd.day+=31;
        case 9:
           sd.day+=30;
        case 8:
           sd.day+=31;
        case 7:
           sd.day+=31;
        case 6:
           sd.day+=30;
        case 5:
           sd.day+=31;
        case 4:
           sd.day+=30;
        case 3:
           sd.day+=31;
        case 2:
           sd.day+=IsLeapYear(sd.year)?29:28;
        case 1:
           sd.day+=31;
    }
    return sd.day;
}
int SDateToAbsDays(struct SDate sd)
{
        int years = sd.year -1;
        int days = years*365 + years/4 - years/100 + years/400;
        days+=DaysOfOneYear(sd);
        return days;
}
struct SDate getDay(char path[256])
{
        char string[256],buf[256],year[10],month[5],day[5];
        FILE *linefirst=NULL;
        struct SDate sd;
        if((linefirst=fopen(path,"r"))!=NULL)
        {
                fgets(string,sizeof(string),linefirst);
                get_year_month_day(string,buf,year,month,day);
                sd.year=atoi(year);
                sd.month=atoi(month);
                sd.day=atoi(day);
        }
        else{
                printf("无法读取临时文件\n");
        }
        return sd;
}
char *right(char *dst,char *src,int n)
{
        char *p=src;
        char *q=dst;
        int len=strlen(src);
        if(n>len)
                n=len;
        p+=(len-n);
        while(*(q++)=*(p++));
        return dst;
}
void  get_year_month_day(char *string,char *buf,char *year,char *month,char *day)
{
        sscanf(string, "%[^ ]", buf);
        sscanf(buf,"%[^/]",year);
        sscanf(buf, "%*[^/]/%[^/]", month);
        right(day,buf,2);
}
int computeDay(char * filename)
{
        char cmd[1024];
        char line1[256]="";
        snprintf(line1,sizeof(line1),"head -1 %s > %sline1",filename,ANTIVIRUS_TEMP_DIR);
        system(line1);
        char linelast[256]="";
        snprintf(linelast,sizeof(linelast),"tail -1 %s >%slinelast",filename,ANTIVIRUS_TEMP_DIR);
        system(linelast);
        struct SDate d1,d2;
        d1=getDay("/usr/share/antivirus/temp/line1");
        d2=getDay("/usr/share/antivirus/temp/linelast");
        printf("最后一行是数据年%d,月%d，日%d\n",d2.year,d2.month,d2.day);
        int day;
        day=SDateToAbsDays(d2)-SDateToAbsDays(d1);
        printf("一共相差%d\n",day);
        snprintf(cmd,sizeof(cmd),"rm -f %sline1",ANTIVIRUS_TEMP_DIR);
        system(cmd);
        memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"rm -f %slinelast",ANTIVIRUS_TEMP_DIR);
        system(cmd);
        return day;
}

int get_upgrde_status( char * file_info)
{
    FILE *pstatusFile;
    char buffer[30];
    char *pbuf;
    char retBuffer[30];
    char *pretBuf =NULL;
    int iAvlStatus = 0;
    char szAvlStatus[10];
    int iUpgradeStatus = 0;


    char file_name[512],file_path[512],read_file_path[1024];
    int i =0 ;
    i = lastIndexOf(file_info,"/");
    if(i > 0 )
    {
        if(i< sizeof(file_path))
        {
            strncpy(file_path,file_info,i);
        }
        snprintf(file_name,sizeof(file_name),"%s_%s",g_current_user,&file_info[i+1]);
        snprintf(read_file_path,sizeof(read_file_path),"%s/%s",file_path,file_name);
    }
    else
    {
        strncpy(read_file_path,file_info,sizeof(read_file_path));
    }


    if((pstatusFile = fopen(read_file_path,"r+")) == NULL)
    {
        return -1;
    }
    pbuf = buffer ;
    fgets(pbuf,30,pstatusFile);
    fclose(pstatusFile);
    if(pbuf != NULL)
    {
        iUpgradeStatus = atoi(pbuf);
        if(iUpgradeStatus < 0)
        {
            iUpgradeStatus = 0;
        }
    }
    return iUpgradeStatus;
}

int checkConnect(char *dst, int cnt)
{
    int i = 0;
    FILE *stream;
    char recvBuf[16] = {0};
    char cmdBuf[256] = {0};

    if (NULL == dst || cnt <= 0)
        return -1;

    sprintf(cmdBuf, "ping %s -c %d -i 0.2 | grep time= | wc -l", dst, cnt);
    stream = popen(cmdBuf, "r");
    fread(recvBuf, sizeof(char), sizeof(recvBuf)-1, stream);
    pclose(stream);
    if (atoi(recvBuf) > 0)
        return 0;
    return -1;
}
void delete_log_30daysago(char* log_file)
{
        char cmd[256];
        snprintf(cmd,sizeof(cmd),"%sdelete_log_30daysago.sh %s",ANTIVIRUS_BIN_DIR,log_file);
        system(cmd);
}

void write_upgrade_log(char *file_info,char *szAvlStatus)
{
    FILE *pstatusFile;
	printf("进入写升级日志函数\n");

    char file_name[512],file_path[512],write_file_path[1024];
    int i =0 ;
    i = lastIndexOf(file_info,"/");
    if(i > 0 )
    {
        if(i< sizeof(file_path))
        {
            strncpy(file_path,file_info,i);
        }
        snprintf(file_name,sizeof(file_name),"%s_%s",g_current_user,&file_info[i+1]);
        snprintf(write_file_path,sizeof(write_file_path),"%s/%s",file_path,file_name);
    }
    else
    {
        strncpy(write_file_path,file_info,sizeof(write_file_path));
    }

    if((pstatusFile = fopen(write_file_path,"abr+")) == NULL)
    {
            return ;
    }
    fwrite(szAvlStatus,strlen(szAvlStatus),1,pstatusFile);
//                fprintf(avl_status_file,"%s\n",szAvlStatus);
    fclose(pstatusFile);
    delete_log_30daysago(write_file_path);

/*    while(1)
        {
		printf("进入升级日志时间判断\n");
                if(computeDay(avl_status_file)>=30)
                {
		printf("计算时间大于30");
		char cmd[256];
                //snprintf(cmd,sizeof(cmd),">%slog_file",ANTIVIRUS_LOG_DIR);
                        snprintf(cmd,sizeof(cmd),"sed -i '1d' %s",avl_status_file);
                        system (cmd);
                }
                else
                {
		printf("计算时间小于30");
                        break;
                }
        }*/



}

int get_antivirus_status()
{
    FILE *pstatusFile;
    char buffer[30];
    char *pbuf;
    char retBuffer[30];
    char *pretBuf =NULL;
    int iAvlStatus = 0;
    char avl_status_file[100], upgrade_status_file[100];
    char szAvlStatus[10];
    snprintf(avl_status_file,100,"%s%s_avlstatus.txt",g_current_user,ANTIVIRUS_TEMP_DIR);
    snprintf(upgrade_status_file,100,"%s%s_avlUpgradeStatus.txt",g_current_user,ANTIVIRUS_TEMP_DIR);
    int avlStatus = get_upgrde_status(upgrade_status_file);
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
    if(iAvlStatus == AVL_INIT_STATUS || iAvlStatus == AVL_SCAN_STATUS || avlStatus == AVL_UPGADE_STATUS )
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


void write_antivirus_status(char *file_info,char *szStatus)
{
    FILE *pstatusFile;
    char szAvlStatus[10];

    char file_name[512],file_path[512],write_file_path[1024];


    int i =0 ;
    i = lastIndexOf(file_info,"/");
    if(i > 0 )
    {
        if(i< sizeof(file_path))
        {
            strncpy(file_path,file_info,i);
        }
        snprintf(file_name,sizeof(file_name),"%s_%s",g_current_user,&file_info[i+1]);
        snprintf(write_file_path,sizeof(write_file_path),"%s/%s",file_path,file_name);
    }
    else
    {
        strncpy(write_file_path,file_info,sizeof(write_file_path));
    }

    if((pstatusFile = fopen(write_file_path,"w+")) == NULL)
    {
        return ;
    }
    strncpy(szAvlStatus,szStatus,sizeof(szAvlStatus));
    fwrite(szAvlStatus,strlen(szAvlStatus),1,pstatusFile);
    fclose(pstatusFile);
}


int main()
{
    char upgradeCmd[256];
    FILE * upgradeId;
    char szLoginfo[512];
    char getVersionCmd[256] = {0};
    char oldVersion[256] = {0};
    char newVersion[256] = {0};
    char upgradeBeginTime[30];
    char upgrade_status_file[100];
    char upgrade_log_file[100];
    char upgrade_log_temp_file[100];
    time_t now;
    setlocale(LC_ALL,"");
    textdomain("antivirus");

    snprintf(upgrade_status_file,100,"%s%s_avlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
    snprintf(upgrade_log_file,100,"%savlUpgradeLog.txt",ANTIVIRUS_LOG_DIR);
    snprintf(upgrade_log_temp_file,100,"%s_avlUpgradeLog_tmp.txt",ANTIVIRUS_TEMP_DIR);


    now = time(0);
    struct tm *tnow = localtime(&now);
    sprintf(upgradeBeginTime,"%04d/%02d/%02d %02d:%02d:%02d", 1900+tnow->tm_year,
             tnow->tm_mon+1,
             tnow->tm_mday,
             tnow->tm_hour,
             tnow->tm_min,
             tnow->tm_sec);


    if (checkConnect("www.baidu.com", 4) == -1 )
    {
        write_antivirus_status(upgrade_status_file,"3");
        snprintf(szLoginfo,sizeof(szLoginfo),"%s,%s,%s,\n",upgradeBeginTime,_("automatic-update"),_("network problems,upgrade fail"));
        write_upgrade_log(upgrade_log_file,szLoginfo);
        write_upgrade_log(upgrade_log_temp_file,szLoginfo);
        return AVL_UPGADE_FAIL;

    }

    sprintf(getVersionCmd, "freshclam -V | cut -d ' ' -f2 | cut -d '/' -f-2");
    upgradeId = popen(getVersionCmd, "r");
    fread(oldVersion, sizeof(char), sizeof(oldVersion)-1, upgradeId);
    pclose(upgradeId);


    sprintf(upgradeCmd, " freshclam > %supgradeInfo.txt 2>&1",ANTIVIRUS_TEMP_DIR);
    upgradeId = popen(upgradeCmd, "r");
    pclose(upgradeId);

    if (checkConnect("www.baidu.com", 4) == -1 )
    {
        write_antivirus_status(upgrade_status_file,"3");
        snprintf(szLoginfo,sizeof(szLoginfo),"%s,%s,%s,\n",upgradeBeginTime,_("automatic-update"),_("network problems,upgrade fail"));
        write_upgrade_log(upgrade_log_file,szLoginfo);
        write_upgrade_log(upgrade_log_temp_file,szLoginfo);
        return AVL_UPGADE_FAIL;

    }

    sprintf(getVersionCmd, "freshclam -V | cut -d ' ' -f2 | cut -d '/' -f-2");
    upgradeId = popen(getVersionCmd, "r");
    fread(newVersion, sizeof(char), sizeof(newVersion)-1, upgradeId);
    pclose(upgradeId);

    if(strcmp(newVersion,oldVersion) == 0)
    {
        write_antivirus_status(upgrade_status_file,"1");
        snprintf(szLoginfo,sizeof(szLoginfo),"%s,%s,%s,\n",upgradeBeginTime,_("automatic-update"),_("now-is-new"));
        write_upgrade_log(upgrade_log_file,szLoginfo);
        write_upgrade_log(upgrade_log_temp_file,szLoginfo);
/*	char last_update_time[200];
        getsystime();
        char year[10],month[10],day[10];

        int in = 0;
        char *update_year_month_day[10];
        update_year_month_day[in] = strtok(scan_starttime," ");
        while(update_year_month_day[in])
        {
                        in++;
                        update_year_month_day[in] = strtok(NULL," ");

        }
        char *year_month_day[10];
        in = 0;
        year_month_day[in] = strtok(update_year_month_day[0],"/");
        while(year_month_day[in])
        {
                        in++;
                        year_month_day[in] = strtok(NULL,"/");

        }

        sprintf(last_update_time,_("you last update is:%syear,%smonth,%sday"),year_month_day[0],year_month_day[1],year_month_day[2]);
        gtk_label_set_text(last_update_label,last_update_time);
        change_font_style((GtkLabel *)last_update_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");*/
        return AVL_NOT_NEED_UPGRADE;
    }


    write_antivirus_status(upgrade_status_file,"2");
    snprintf(szLoginfo,sizeof(szLoginfo),"%s,%s,%s,\n",upgradeBeginTime,_("automatic-update"),_("upgrade success"));
    write_upgrade_log(upgrade_log_file,szLoginfo);
    write_upgrade_log(upgrade_log_temp_file,szLoginfo);
/*    char last_update_time[200];
        getsystime();
        char year[10],month[10],day[10];

        int in = 0;
        char *update_year_month_day[10];
        update_year_month_day[in] = strtok(scan_starttime," ");
        while(update_year_month_day[in])
        {
                        in++;
                        update_year_month_day[in] = strtok(NULL," ");

        }
        char *year_month_day[10];
        in = 0;
        year_month_day[in] = strtok(update_year_month_day[0],"/");
        while(year_month_day[in])
        {
                        in++;
                        year_month_day[in] = strtok(NULL,"/");

        }
        sprintf(last_update_time,_("you last update is:%syear,%smonth,%sday"),year_month_day[0],year_month_day[1],year_month_day[2]);
        gtk_label_set_text(last_update_label,last_update_time);
        change_font_style((GtkLabel *)last_update_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");*/
    return AVL_UPGADE_SUCESS;
}
