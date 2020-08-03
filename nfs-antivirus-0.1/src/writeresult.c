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
#include <locale.h>
#include <libintl.h>
#include <string.h>
#include <stdlib.h>
#include "antivirus.h"
#include "scan.h"
#include "function.h"
#include "writeresult.h"
#define _(STRING) gettext(STRING)
extern char scan_starttime[30];
struct SDate
{
    	short int year;
    	short int month;
    	short int day;
};
int IsLeapYear(short year);
int DaysOfOneYear(struct SDate sd);
int SDateToAbsDays(struct SDate sd);
void get_year_month_day(char *string,char *buf,char *year,char *month,char *day);
extern char g_current_user[256]; //当前启动杀毒软件用户
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
struct SDate getDay();

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

void  get_year_month_day(char *string,char *buf,char *year,char *month,char *day)
{
	sscanf(string, "%[^ ]", buf);
	sscanf(buf,"%[^/]",year);
        sscanf(buf, "%*[^/]/%[^/]", month);
        right(day,buf,2);
}
void delete_log_30daysago(char* log_file)
{
        char cmd[256];
        char log_file_dir[1024];
        snprintf(log_file_dir,sizeof(log_file_dir),"%s%s_%s",ANTIVIRUS_LOG_DIR,g_current_user,log_file);
        printf("sssssssssss\n");
        snprintf(cmd,sizeof(cmd),"%sdelete_log_30daysago.sh %s",ANTIVIRUS_BIN_DIR,log_file_dir);
        system(cmd);
}

void writelog(int logType)
{    
    	setlocale(LC_ALL,"");
   	textdomain("antivirus");
    	FILE *LogFile;
    	char LogBuf[1024];
    	char scantype[30];
    	int threatNum=0;
    	char result[200];
	char file_temp[1024];
	char cmd[1024];
	snprintf(file_temp,sizeof(file_temp),"%s%s_log_file",ANTIVIRUS_LOG_DIR,g_current_user);
    	if((LogFile = fopen(file_temp,"abr+")) == NULL)
    	{
        	return;
    	}
    	if(get_scanType()== ALL_SCAN)
    	{
        	strncpy(scantype,_("all-scan"),sizeof(scantype));
    	}
    	if(get_scanType()== FAST_SCAN)
    	{
		strncpy(scantype,_("quick-scan"),sizeof(scantype));
    	}
    	if(get_scanType()== CUSTOM_SCAN)
    	{
        	strncpy(scantype,_("custom-scan"),sizeof(scantype));
        
    	}
/*    	if(get_scanType()== SPECIFIED_SCAN)
    	{
        	strncpy(scantype,_("usb-scan"),sizeof(scantype));
    	}
    	if(get_scanType()== FIREFOX_DOWNLOAD_SCAN)
    	{
        	strncpy(scantype,_("download scan"),sizeof(scantype));
    	}*/

    	threatNum=getThreadNum();
    	if(threatNum==0)
        {
                if(logType == QUIT_SCAN_LOG_TYPE )
                {
                    strncpy(result,_("scan quit no thread"),sizeof(result));
                }
                else
                {
                    strncpy(result,_("complete no thread"),sizeof(result));

                }
        }
    	else if(threatNum>0)
        {
                if(logType == QUIT_SCAN_LOG_TYPE )
                {
                    snprintf(result,sizeof(result),_("scan quit found %d thread items"),threatNum);
                }
                else
                {
                    snprintf(result,sizeof(result),_("complete found %d thread items"),threatNum);
                }

        }
	if(get_scanType()!=0)
        {
                snprintf(LogBuf,sizeof(LogBuf),"%s,%s,%s,",scan_starttime,scantype,result);       
                fprintf(LogFile,"%s\n",LogBuf);
        }
        fclose(LogFile);
        delete_log_30daysago("log_file");

/*	while(1)
        {
                if(computeDay(file_temp)>=30)
                {
                //snprintf(cmd,sizeof(cmd),">%slog_file",ANTIVIRUS_LOG_DIR);
                        printf("sssssssssssss\n");
                        snprintf(cmd,sizeof(cmd),"sed -i '1d' %slog_file",ANTIVIRUS_LOG_DIR);
                        system (cmd);
                }
                else
                {
                        printf("s2222222222222222222\n");
                        break;
                }
        }
*/
        /*if(computeDay(file_temp)>=30){
		snprintf(cmd,sizeof(cmd),">%slog_file",ANTIVIRUS_LOG_DIR);
		system (cmd);
	}*/
}

void write_defence_log(char *defence_log)
{
        setlocale(LC_ALL,"");
        textdomain("antivirus");
        FILE *LogFile;
	char log_file_temp[1024];
	snprintf(log_file_temp,sizeof(log_file_temp),"%s%s_defence_log",ANTIVIRUS_LOG_DIR,g_current_user);
        if((LogFile = fopen(log_file_temp,"abc+")) == NULL)
        {
                return;
        }
/*        char defence_log[1024];
        char *defence_log_type=_("defence");
        char *defenc_log_result;
        getsystime();
        if(open_or_close==1)
	{
                defenc_log_result = _("open");
        }
        else
	{
                defenc_log_result=_("close");
        }
        snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);*/
        fprintf(LogFile,"%s\n",defence_log);
        fclose(LogFile);
	/*char cmd[1024];
	while(1)
        {
                if(computeDay(log_file_temp)>=30)
                {
                        snprintf(cmd,sizeof(cmd),"sed -i '1d' %sdefence_log",ANTIVIRUS_LOG_DIR);
                        system (cmd);
                }
                else
                {
                        break;
                }
        }
*/
	/*if(computeDay(log_file_temp)>=30){
                snprintf(cmd,sizeof(cmd),">%s./defence_log",ANTIVIRUS_LOG_DIR);
                system (cmd);
        }*/
/*	snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
        fprintf(LogFile,"%s\n",defence_log);
        fclose(LogFile);*/
	
}

