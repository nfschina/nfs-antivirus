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
#include<stdio.h>  
#include<pthread.h>  
#include<string.h>  
#include<sys/types.h>  
#include<unistd.h>  
#include <gtk/gtk.h>
#include <locale.h>
#include <libintl.h>
#include <sys/stat.h>
#include "detect_usb.h"
#include "../page/setting-page.h"
#include "../scan.h"
#include "../function.h"
#include "../antivirus.h"
#include "../result.h"
#include "../writeresult.h"
#define _(STRING) gettext(STRING)
pthread_t ntid;  
//extern GtkWindow *window ,*detail_result_table;
extern GtkWindow *window ,*detail_result_table,*real_protect_result_table,*table1,*table2,*table3,*table4,*table6,*real_protect_result_list;
extern int area;
extern char scan_starttime[30];
extern char g_current_user[256]; //当前启动杀毒软件用户

int  write_real_time_protect_result()
{
    printf("进入写是实时防御日志的位置\n");
    char filepath[100];
    char buf[1024];
    char resultFilePath[256];
    FILE *fp ;
    char cmd[256];
    char defence_log[1024];
    //char *defence_log_type=_("real protect scan");
    char defence_log_type[1024];
    strcpy(defence_log_type,_("real protect scan"));
    char defenc_log_result[1024];

    //snprintf(filepath,100,"%s%s_%s",ANTIVIRUS_LOG_DIR,g_current_user,"clamd.log");
	snprintf(filepath,100,"%s%s",ANTIVIRUS_LOG_DIR,"clamd.log");
    
    fp = fopen(filepath,"r+");
    if(fp == NULL)
    {
//	    defenc_log_result=_("real protect no thred");
	    strcpy(defenc_log_result,_("real protect no thred"));
	    snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
            write_defence_log(defence_log);
	    delete_log_30daysago("defence_log");

            return -1;
    }
    snprintf(cmd,256,"rm -f %sscan_results.txt",ANTIVIRUS_TEMP_DIR);
    system(cmd);
    //获取扫描开始时间
    getsystime();
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"echo %s >%s%s_scan_time.txt",ANTIVIRUS_TEMP_DIR,scan_starttime,g_current_user);
    system(cmd);
    while(fgets(buf,1023,fp) != NULL)
    {

         int in=0;
         char *p[20];
         char *buffer = buf;
         int j = 0;
         while((p[in]=strtok(buffer,","))!=NULL)
         {
             in++;
             buffer=NULL;
         }
         char fileName[1024],threatName[1024];     
         strncpy(fileName,p[0],1024);
         strncpy(threatName,p[1],1024);   
         write_log(fileName,threatName);
	 snprintf(defenc_log_result,sizeof(defenc_log_result),"%s %s",fileName,threatName);
         snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
         write_defence_log(defence_log);
         printf("编写完成\n");
    }
    delete_log_30daysago("defence_log");
    fclose(fp);
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"rm -f %sclamd.log",ANTIVIRUS_LOG_DIR);
    system(cmd);
    return 1;
}

int check_threat_in_result(char * filePath)
{
    char buf[1024];
    char resultFilePath[256];
    snprintf(resultFilePath,256,"%sscan_results.txt",ANTIVIRUS_TEMP_DIR);
    FILE *fp=fopen(resultFilePath,"rw");
    if(fp == NULL)
    {
            return 1;
    }
    while(fgets(buf,1023,fp) != NULL)
    {
                int in=0;
                char *p[20];
                char *buffer = buf;
                int j = 0;
                if(buffer ==NULL)
                {
                    fclose(fp);
                    return 1;
                }
                while((p[in]=strtok(buffer,";"))!=NULL)
                {
                        in++;
                        buffer=NULL;
                }

                if (strlen(p[1])>0)
                {
                        if(strcmp(filePath,p[1])==0)
                        {
                                fclose(fp);
                                return 0;
                        }
                }
    }
    fclose(fp);
    return 1;

}

int check_if_scan_threat()
{
    char filepath[100];
    char buf[1024];
    FILE *fp ;
    char cmd[1024] ;

    //snprintf(filepath,100,"%s%s_%s",ANTIVIRUS_LOG_DIR,g_current_user,"clamd.log");
	snprintf(filepath,100,"%s%s",ANTIVIRUS_LOG_DIR,"clamd.log");

    fp = fopen(filepath,"r+");
    if(fp == NULL)
    {
            return -1;
    }
    while(fgets(buf,1023,fp) != NULL)
    {

         int in=0;
         char *p[20];
         char *buffer = buf;
         int j = 0;
         while((p[in]=strtok(buffer,","))!=NULL)
         {
             in++;
             buffer=NULL;
         }
         if(strlen(p[0]) > 0)
         {
             int check_ret = 0;
             check_ret = check_threat_in_result(p[0]);
             if(check_ret != 1)
             {
                 char specialFilePath[1024];
                 strncpy(specialFilePath,p[0],1024);
                 replace_string(specialFilePath,"/","\\\/");
                 snprintf(cmd,1024,"sed -i /\"%s\"/d %s",specialFilePath,filepath);
                 system(cmd);
             }
         }

    }
    fclose(fp);
    if((fp = fopen(filepath,"r+"))!=NULL)
    {
         char ch;
         ch=fgetc(fp);
         if(ch!=EOF)
         {
             fclose(fp);
             return 1;
         }
    }

    return 0;
}

void check_real_time_protection()
{
     char filepath[100], cmd[256],avlStatusFilepath[100];
     FILE *dev_path;
	 char set_file_path[1024];
     snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);

     int check_real_time_status  = get_antivirus_setting_item_inttype("check-real-time-protect",set_file_path);

    // snprintf(filepath,100,"%s%s_%s",ANTIVIRUS_LOG_DIR,g_current_user,"clamd.log");
	 snprintf(filepath,100,"%s%s",ANTIVIRUS_LOG_DIR,"clamd.log");

     if(check_real_time_status != 1)
     {
         return;
     }
     
     if((dev_path=fopen(filepath,"r+"))!=NULL)
     {
                char ch;
                ch=fgetc(dev_path);
                if(ch!=EOF)
                {
                        fclose(dev_path);
                        int avlStatus = 0;
                        snprintf(avlStatusFilepath,100,"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
                        avlStatus = get_upgrde_status(avlStatusFilepath);
                        while( avlStatus == 3 ||  avlStatus == 4  || avlStatus == 6 || avlStatus == 1 )
                        {
                            sleep(2);
                            avlStatus = get_upgrde_status(avlStatusFilepath);
                        }
                        memset(cmd,0,sizeof(cmd));
                        //sprintf(cmd,"sed -i /.tmp/d %s%s_clamd.log",ANTIVIRUS_LOG_DIR,g_current_user);
                        sprintf(cmd,"sed -i /.tmp/d %sclamd.log",ANTIVIRUS_LOG_DIR);

                        system(cmd);
                        int ret = 0;
                        ret = check_if_scan_threat();
                        if(ret != 1)
                        {
                            memset(cmd,0,sizeof(cmd));
                            //sprintf(cmd,"rm -f %s%s_clamd.log",ANTIVIRUS_LOG_DIR,g_current_user);
							sprintf(cmd,"rm -f %sclamd.log",ANTIVIRUS_LOG_DIR);
                            system(cmd);
                            return ;

                        }

                        memset(cmd,0,sizeof(cmd));
                        sprintf(cmd,"%sreal_protect_dialog %s",ANTIVIRUS_BIN_AUTO_DEFENSE_DIR,g_current_user);
                        system(cmd);

                        FILE *sureFile;
                        char sureFilePath[256];
                        sprintf(sureFilePath,"%s%s_real_time_sure.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
                        if(( sureFile = fopen(sureFilePath,"r+"))!=NULL)
                        {
							printf("aaaaaaaaaaaa\n");
                            char surechOne;
                            surechOne = fgetc(sureFile);
                            if(surechOne != EOF)
                            {
                                 hide_other_page();
                                 write_real_time_protect_result();

                                 if(gtk_widget_get_visible(detail_result_table))
                                     gtk_widget_hide(detail_result_table);

					if(area==1){
                                        gtk_widget_hide(table1);

                                }
                                if(area==2){
                                        gtk_widget_hide(table2);
                                }
                                if(area==3){
                                        gtk_widget_hide(table3);
                                }
                                if(area==4){
                                        gtk_widget_hide(table4);
                                }
                                if(area==6){
                                        gtk_widget_hide(table6);
                                }
                                gtk_tree_view_set_model(real_protect_result_list,createModel());
                                area = 10;
                                gtk_widget_show(real_protect_result_table);

                                char redImage[255];
                                snprintf(redImage,255,_("%sred.png"),ANTIVIRUS_IMAGE_DIR);
                                notify_pop(_("anti-virus"),_("Please click on the antivirus treatment"),redImage);

                                memset(cmd,0,sizeof(cmd));
                                sprintf(cmd,"rm -f %s%s_real_time_sure.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
                                system(cmd);

                            }
                            fclose(sureFile);
                        }
                }

    }

}

void check_firefox_download()
{
	//system("rm  -rf /tmp/firefox_download*");
        //system("rm /tmp/.firefox_info");
    	char cmd[256];
        //char filepath[100]="/tmp/.firefox_info";
        //char tmpfile[100]="/tmp/.tmp_firefox_info";
		char filepath[100]="/tmp/cdos_download.tmp";
		char tmpfile[100]="/tmp/.tmp_cdos_download.tmp";

	FILE *dev_path;
	if(access(filepath,0) == -1 )
        {
	    return ;
	}

        snprintf(cmd,256,"bash %sdeal_firefox_download.sh",ANTIVIRUS_BIN_AUTO_DEFENSE_DIR);
        system(cmd);
        memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"head -n 1 %s > %s",filepath,tmpfile);
        system(cmd);	
        char temp_downloadpath[1024],scan_downloadpath[1024],avlStatusFilepath[100];
        
		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);

        if((dev_path=fopen(tmpfile,"r+"))!=NULL)
        {
        	char ch;
                ch=fgetc(dev_path);
                if(ch!=EOF)
                {
                        snprintf(avlStatusFilepath,100,"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
                        fseek(dev_path,0,SEEK_SET);
                        fgets(temp_downloadpath,sizeof(temp_downloadpath),dev_path);
                        temp_downloadpath[strlen(temp_downloadpath)-1]=0;
                        strncpy(scan_downloadpath,temp_downloadpath,sizeof(temp_downloadpath));
                        int avlStatus = 0;
                        avlStatus = get_upgrde_status(avlStatusFilepath);
                        while( avlStatus == 3 || avlStatus == 4 || avlStatus == 6 || avlStatus == 1 )
                        {
                            sleep(2);
                            avlStatus = get_upgrde_status(avlStatusFilepath);
                        }

                        int detectStatus = -1 ;
                        detectStatus = get_antivirus_setting_item_inttype("auto-check-download",set_file_path);
                        if(detectStatus == 1)
                        {
                            gdk_threads_enter();
                            specified_scan_type(scan_downloadpath,FIREFOX_DOWNLOAD_SCAN);
                            sleep(1);
                            gdk_threads_leave();
                        }
                        char rm_firstline[100];
                        snprintf(rm_firstline,sizeof(rm_firstline),"sed -i '1d' %s",filepath);
                        system(rm_firstline);
                   }

                   fclose(dev_path);

                }

}

void check_antivirus_upgrade()
{
        char cmd[256];
        char filepath[100];
        FILE *dev_path;
        char upgradeInfo[100];
        char upgradeImage[100];
        snprintf(filepath,100,"%s%s_avlUpgradeLog_tmp.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
        if((dev_path=fopen(filepath,"r+"))!=NULL)
        {
                char ch;
                ch=fgetc(dev_path);
                if(ch!=EOF)
                {
                        fseek(dev_path,0,SEEK_SET);
                        fgets(upgradeInfo,sizeof(upgradeInfo),dev_path);
                        upgradeInfo[strlen(upgradeInfo)-1] = 0;

                        snprintf(upgradeImage,100,_("%supdate_setting.png"),ANTIVIRUS_IMAGE_DIR);
                        notify_pop(_("anti-virus"),upgradeInfo,upgradeImage);

                        snprintf(cmd,sizeof(cmd),"rm -f %s%s_avlUpgradeLog_tmp.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
                        system(cmd);
                }

                fclose(dev_path);

         }

}


void check_click_scan_file()
{
    char cmd[256];
    char filepathName[100]=".click_scan_file_info.txt";
    char tmpfileName[100]=".click_scan_file_info_temp.txt";
    char fullfileName[512];
    char tmpfullfileName[512];
    FILE *dev_path;
    char pathName[256];
    struct stat buf;
    int scan_type = CLICK_DIR_SCAN;

    if(access(filepathName,0) == -1 )
    {
        return ;
    }


    snprintf(tmpfullfileName,512,"/home/%s/%s",g_current_user,tmpfileName);
    snprintf(fullfileName,512,"/home/%s/%s",g_current_user,filepathName);

    snprintf(cmd,sizeof(cmd),"head -n 1 %s > %s",fullfileName,tmpfullfileName);
    system(cmd);
    char temp_scan_file_path[1024],scan_file_path[1024];
    if((dev_path=fopen(tmpfullfileName,"r+"))!=NULL)
    {
        char ch;
        ch=fgetc(dev_path);
        if(ch!=EOF)
        {
            fseek(dev_path,0,SEEK_SET);
            fgets(temp_scan_file_path,sizeof(temp_scan_file_path),dev_path);
            temp_scan_file_path[strlen(temp_scan_file_path)-1]=0;
            strncpy(scan_file_path,temp_scan_file_path,sizeof(temp_scan_file_path));
            char avlStatusFilePath[512];
            snprintf(avlStatusFilePath,512,"%s%s",ANTIVIRUS_TEMP_DIR,"avlstatus.txt");
            int r = lstat(scan_file_path, &buf);
            if( r == 0 )
            {
                if( buf.st_mode & S_IFDIR )
                {
                    scan_type = CLICK_DIR_SCAN;
                }
                else
                {
                    scan_type = CLICK_FILE_SCAN;
                }
            }



            int avlStatus = 0;
            avlStatus = get_upgrde_status(avlStatusFilePath);
            while( avlStatus == 3 || avlStatus == 4 || avlStatus == 6 || avlStatus == 1 )
            {
                sleep(2);
                avlStatus = get_upgrde_status(avlStatusFilePath);
            }

            gdk_threads_enter();
            specified_scan_type(scan_file_path,scan_type);
            sleep(1);
            gdk_threads_leave();
            char rm_firstline[100];
            snprintf(rm_firstline,sizeof(rm_firstline),"sed -i '1d' %s",fullfileName);
            system(rm_firstline);
        }
        fclose(dev_path);
    }

}



void thread(void *arg)
{
        char avlStatusFilePath[100];
        snprintf(avlStatusFilePath,100,"%s%s",ANTIVIRUS_TEMP_DIR,"avlstatus.txt");

        char set_file_path[1024];
        snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);


        char filepath[50]="/tmp/.antivirus/log/usbdir";
        char tmpfile[50]="/tmp/.antivirus/log/tmp";


        char cmd[256];
        char time_antivirus_path[50];
        FILE *dev_path= NULL,*ptime_antivirus=NULL;
        char scan_usbpath[512],temp_usbpath[512];
	printf("in timing aaaaaaaaaaa\n");

        while(1)
        {
                dev_path= NULL;
                ptime_antivirus=NULL;
                memset(scan_usbpath,sizeof(scan_usbpath),0);
                memset(temp_usbpath,sizeof(temp_usbpath),0);
		printf("filepath is aaaaaaaaaaaa:%s\n",filepath);
                if(access(filepath,0) != -1 )
                {
                     memset(cmd,sizeof(cmd),0);
                     snprintf(cmd,sizeof(cmd),"head -n 1 %s > %s",filepath,tmpfile);
                     system(cmd);
		     
                     if(access(tmpfile,0) != -1 )
                     {

                         if((dev_path=fopen(tmpfile,"r+"))!=NULL)
                         {
                            char ch;
                            ch=fgetc(dev_path);
                            if(ch!=EOF)
                            {
                                fseek(dev_path,0,SEEK_SET);
                                fgets(temp_usbpath,sizeof(temp_usbpath),dev_path);
                                temp_usbpath[strlen(temp_usbpath)-1]=0;
                                strncpy(scan_usbpath,temp_usbpath,sizeof(temp_usbpath));
                                int avlStatus = 0;
                                avlStatus = get_upgrde_status(avlStatusFilePath);
				printf("avlStaus is :%d \n",avlStatus);
                                while( avlStatus == 3 ||  avlStatus == 4  || avlStatus == 6 || avlStatus == 1 )
                                {
                                    sleep(2);
                                    avlStatus = get_upgrde_status(avlStatusFilePath);
                                }

                                int detectStatus = -1 ;
                                detectStatus = get_antivirus_setting_item_inttype("auto-check-mobile-dev",set_file_path);
                                if(detectStatus == 1)
                                {
                                    gdk_threads_enter();
                                    printf("scan_usbpath is :%s\n",scan_usbpath);
                                    specified_scan_type(scan_usbpath,SPECIFIED_SCAN);
                                    sleep(1);
                                    gdk_threads_leave();
                                }
                                char rm_firstline[100];
                                snprintf(rm_firstline,sizeof(rm_firstline),"sed -i '1d' %s",filepath);
                                system(rm_firstline);
                           }

                           fclose(dev_path);

                        }

                     }


                }

                memset(time_antivirus_path,sizeof(time_antivirus_path),0);

                snprintf(time_antivirus_path,50,"%s.%s_time_antivirus.txt",ANTIVIRUS_TEMP_DIR,g_current_user);

		printf("time_antiviurs_path is :%s\n",time_antivirus_path);
                if(access(time_antivirus_path,0) != -1 && ( ptime_antivirus =fopen(time_antivirus_path,"r+"))!=NULL)
                {
                    char ch;
                    char scan_type[30];
                    ch=fgetc(ptime_antivirus);
                    if(ch!=EOF)
                    {
                        fseek(ptime_antivirus,0,SEEK_SET);
                        fgets(scan_type,sizeof(scan_type),ptime_antivirus);
                        scan_type[strlen(scan_type)-1]=0;
                        char cmd[512];
                        sprintf(cmd,"rm -rf %s",time_antivirus_path);
                        system(cmd);
                        int avlStatus = 0;
                        avlStatus = get_upgrde_status(avlStatusFilePath);
			printf("avlStatus aaaaaaaaaaaaaaa:%d,%s\n",avlStatus,scan_type);
                        while( avlStatus == 3 || avlStatus == 4 ||  avlStatus == 6 || avlStatus == 1 )
                        {
                                sleep(2);
                                avlStatus = get_upgrde_status(avlStatusFilePath);
                        }

                        if(strcmp(scan_type, "quick-scan")==0 )
                        {
                                gdk_threads_enter();
                                execution_scan(FAST_SCAN,NULL);
                                sleep(1);
                                gdk_threads_leave();
                        }
                        else if(strcmp(scan_type , "all-scan") == 0)
                        {
                                gdk_threads_enter();
                                execution_scan(ALL_SCAN,NULL);
                                sleep(1);
                                gdk_threads_leave();
                        }
                    }
                    fclose(ptime_antivirus);

                }
                check_firefox_download();
                //判读是否升级了病毒引擎
                check_antivirus_upgrade();

                check_real_time_protection();

                check_click_scan_file();

                sleep(2);
        }

}

int create_usb_detect_thread()  
{  
        int ret;  
        sleep(45);
        g_thread_new ("usb_detect",thread,NULL);
        sleep(2);  
        return 0;  
}  
