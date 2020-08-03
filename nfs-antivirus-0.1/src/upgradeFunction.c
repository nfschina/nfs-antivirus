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
#include <gtk/gtk.h>
#include <sys/stat.h>
#include "upgradeFunction.h"
#include "antivirus.h"
#include "function.h"
#include "./page/upgrade-page.h"
#include "upgrade/upgradeself.h"
#define _(STRING) gettext(STRING)
extern GtkWidget *dialog;
extern GtkWidget *isnewlabel;
int clamav_version_new = 0;
int clamav_thread_new= 0;
int clamav_version_label_button_status=0;
int clamav_thread_label_button_status=0;
int engin_is_new_now=0;
extern GtkWidget *update_status_image,*fix,*update_layout;
extern int avlInit;
extern GtkWidget *last_update_label;
extern GtkLabel * attentionlabel;
extern char scan_starttime[30];


int iUpdateEngineFlag = 0 , iUpdateAntivirusFlag = 0;

//dsj-0610
/*#define ENGINE_NOW_VER          "1.0.16"
#define ENGINE_HIGH_VER         "1.0.16"
#define ENGINE_LIB_NOW_VER      "15611"
#define ENGINE_LIB_HIGH_VER     "15611"
#define ENGINE_MAIN_PAGE_VER    "15611"*/
//dsh-end

void delete_updatelog_30daysago(char* log_file)
{
        char cmd[256];
        snprintf(cmd,sizeof(cmd),"%sdelete_log_30daysago.sh %s",ANTIVIRUS_BIN_DIR,log_file);
        system(cmd);
}

void write_upgrade_log(char *avl_status_file,char *szAvlStatus)
{
    	FILE *pstatusFile;
    	char cmd[256];
    	if((pstatusFile = fopen(avl_status_file,"a+")) == NULL)
    	{
        	return ;
    	}
	fwrite(szAvlStatus,strlen(szAvlStatus),1,pstatusFile);
        fclose(pstatusFile);
        delete_updatelog_30daysago(avl_status_file);

}
int get_upgrde_status( char * avl_status_file)
{
    	FILE *pstatusFile;
    	char buffer[30];
    	char *pbuf;
    	char retBuffer[30];
    	char *pretBuf =NULL;
    	int iAvlStatus = 0;
    	char szAvlStatus[10];
    	int iUpgradeStatus = 0;
    	if((pstatusFile = fopen(avl_status_file,"r+")) == NULL)
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
void avlUpgradeLibRefresh(struct gradeInfo *grade_info)
{
	clamav_version_label_button_status=0;
	clamav_thread_label_button_status=0;
	int iCurrentAntiVer=0, iAptAntiVer =0;
	char status_temp[1024];
	char image_temp[1024];
	snprintf(status_temp,sizeof(status_temp),"%savlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR);
	char update_step_status_file[512];
        snprintf(update_step_status_file,sizeof(update_step_status_file),"%savlUpgradeStartStatus.txt",ANTIVIRUS_TEMP_DIR);

#if 0
/**
  *获取当前杀毒软件版本*
**/

	char anti_engine_ver_now[512];
	cmd_shell_result("bak=`dpkg -s cdos-antivirus | grep Version | cut -d ':' -f2 | cut -d '-' -f2`;version=1.0.$bak;echo $version",anti_engine_ver_now,512);
	anti_engine_ver_now[strlen(anti_engine_ver_now)-1]=0;
        printf("当前系统的杀毒软件版本号为%s\n",anti_engine_ver_now);

	if(strlen(anti_engine_ver_now) > 0)
	{
		replace_string(anti_engine_ver_now,".","");
		iCurrentAntiVer = atoi(anti_engine_ver_now);
	        printf("shuzi %d\n",atoi(anti_engine_ver_now));
        }
/**
  *获取apt源杀毒软件的最高版本
**/

	char anti_engine_high_ver[512];
        cmd_shell_result("ver_temp=`apt-cache show cdos-antivirus | grep Version |head -n 1 | cut -d '-' -f2`;ver=1.0.$ver_temp;echo $ver",anti_engine_high_ver,512);
        anti_engine_high_ver[strlen(anti_engine_high_ver)-1]=0;
        printf("当前系统源中杀毒软件版本为%s\n",anti_engine_high_ver);

	 if(strlen(anti_engine_high_ver) > 0)
	 {
	    replace_string(anti_engine_high_ver,".","");

	    iAptAntiVer = atoi(anti_engine_high_ver);
	    printf("shuzi high %d\n",atoi(anti_engine_high_ver));

         }
/**
  *获取病毒库当前版本
**/
	char engine_db_ver_now[512];
	cmd_shell_result("dpkg -s cdos-virus | grep Description|cut -d ':' -f2 |sed s/[[:space:]]//g",engine_db_ver_now,sizeof(engine_db_ver_now));
        engine_db_ver_now[strlen(engine_db_ver_now)-1]=0;	
//	strncpy(engine_db_ver_now,ENGINE_LIB_NOW_VER,sizeof(engine_db_ver_now));	
        printf("engine_db_ver_now is :%s\n",engine_db_ver_now);

/**
  *获取服务器病毒库当前版本
**/
	char engine_db_server_ver[512];
        int run_check_sec_label=0;
/**
  *获取apt源软件版本
**/
	char apt_clamav_ver[512];
	strncpy(apt_clamav_ver,ENGINE_NOW_VER,sizeof(apt_clamav_ver));


///////////                     新加的                        //////////////////////////
/**
  *获取apt源软件版本--1.0.1
**/
	char apt_virus_db_ver[512];
	memset(apt_virus_db_ver,0,sizeof(apt_virus_db_ver));
	//cmd_shell_result("ver_temp=`apt-cache show cdos-virus | grep Version |head -n 1 | cut -d '-' -f2`;ver=1.0.$ver_temp;echo $ver",apt_virus_db_ver,sizeof(apt_virus_db_ver));
	
	cmd_shell_result("ver_temp=`apt-cache show cdos-virus | grep Version |head -n 1 | cut -d '-' -f2|cut -d '+' -f1 `;ver=$ver_temp;echo $ver",apt_virus_db_ver,sizeof(apt_virus_db_ver));
	apt_virus_db_ver[strlen(apt_virus_db_ver)-1]='\0';

	printf("apt_virus_db_ver is :%s\n",apt_virus_db_ver);
/**
  *获取系统中的软件版本
**/

	char sys_virus_ver[512];
	memset(sys_virus_ver,0,sizeof(sys_virus_ver));
	//cmd_shell_result("bak=`dpkg -s cdos-virus | grep Version | cut -d ':' -f2 | cut -d '-' -f2`;version=1.0.$bak;echo $version",sys_virus_ver,sizeof(sys_virus_ver));
	
	cmd_shell_result("bak=`dpkg -s cdos-virus | grep Version | cut -d ':' -f2 | cut -d '-' -f2`;version=$bak;echo $version",sys_virus_ver,sizeof(sys_virus_ver));
	sys_virus_ver[strlen(sys_virus_ver)-1]='\0';
	printf("sys_virus_ver is :%s\n",sys_virus_ver);
/**
  *获取apt源中的病毒库的最高版本
**/
	char apt_engin_db_ver[512];
	memset(apt_engin_db_ver,0,sizeof(apt_engin_db_ver));
	strncpy(apt_engin_db_ver,engine_db_ver_now,sizeof(apt_engin_db_ver));

	printf("apt_engin_db_ver is :%s\n", apt_engin_db_ver);


	#endif



////////////////////////////////////////////////////////////////////////////////////////	
	char anti_engine_ver[1024];
	char anti_engine_check_label[1024];
	char anti_engine_update_cmd[1024];
    	int iUpgradeStatus = 0;
	long int n=0;
	long int m=0;
	
	char cvd_version_cmd[1024];
        char cvd_version_result[1024];
	char engine_db_ver[512];
	char anti_upgrade_info_txt[512];
	snprintf(anti_upgrade_info_txt,sizeof(anti_upgrade_info_txt),"%supgradeInfo.txt",ANTIVIRUS_TEMP_DIR);
    	while(1)
    	{
       		printf("()()()()()()()()(进入升级界面的的线程部分)()()()()()()()():%d\n",iUpgradeStatus);

        	sleep(1);

                iUpgradeStatus = get_upgrde_status(status_temp);

                if(iUpgradeStatus ==1 || iUpgradeStatus ==2)
                {
                    char resultInfo[512];
                    if(iUpdateEngineFlag == 0)
                    {
                       strncpy(resultInfo,_("engine is new"),sizeof(resultInfo));
                    }
                    else if(iUpdateEngineFlag > 0)
                    {
                        strncpy(resultInfo,_("update engine success"),sizeof(resultInfo));
                    }
                    else
                    {
                        strncpy(resultInfo,_("update engine fail"),sizeof(resultInfo));
                    }
                    if(iUpdateAntivirusFlag == 0)
                    {
                       strcat(resultInfo,_(", antivirus is new"));
                    }
                    else if(iUpdateAntivirusFlag > 0)
                    {
                        strcat(resultInfo,_(", update antivirus success"));
                    }
                    else
                    {
                        strcat(resultInfo,_(", update antivirus fail"));
                    }

		    printf("resultInfo is :%s\n",resultInfo);
                    gdk_threads_enter();
			gtk_widget_hide(updating_box);	
	    		gtk_label_set_text(update_net_result_label,_(resultInfo));
			memset(image_temp,0,sizeof(image_temp));
			snprintf(image_temp,sizeof(image_temp),_("%supdate_success.png"),ANTIVIRUS_IMAGE_DIR);
            		gtk_image_set_from_file(GTK_WIDGET(update_fail_success_image),image_temp);
            		change_font_style(update_net_result_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");
			gtk_widget_show(update_result_box);
			if(!gtk_widget_get_visible(update_net_result_label))
			{
				gtk_widget_show(update_net_result_label);
			}
			gdk_threads_leave();
            		break;       
 
                }
                else if(iUpgradeStatus == 3)
        	{
//			printf("网络连接失败,结果界面显示\n");
			gdk_threads_enter();
			gtk_widget_hide(updating_box);
	    		gtk_label_set_text(update_net_result_label,_("network problems,upgrade fail"));
			memset(image_temp,0,sizeof(image_temp));
			snprintf(image_temp,sizeof(image_temp),_("%supdate_warn.png"),ANTIVIRUS_IMAGE_DIR);
            		gtk_image_set_from_file(update_fail_success_image,image_temp);
            		change_font_style(update_net_result_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");
			gtk_widget_show(update_result_box);
			gtk_widget_show(update_net_result_label);
			gdk_threads_leave();
            		break;
        	}
#if 0
        	iUpgradeStatus = get_upgrde_status(status_temp);
        	if( iUpgradeStatus == 1)
        	{
			printf("四、结果页面显示：当前已为最新版本,无需升级\n");
			gdk_threads_enter();
			gtk_widget_hide(updating_box);	
	    		gtk_label_set_text(update_net_result_label,_("now-is-new"));
			memset(image_temp,0,sizeof(image_temp));
			snprintf(image_temp,sizeof(image_temp),_("%supdate_success.png"),ANTIVIRUS_IMAGE_DIR);
            		gtk_image_set_from_file(GTK_WIDGET(update_fail_success_image),image_temp);
            		change_font_style(update_net_result_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");
			gtk_widget_show(update_result_box);
			if(!gtk_widget_get_visible(update_net_result_label))
			{
				gtk_widget_show(update_net_result_label);
			}
			gdk_threads_leave();
            		break;
        	}
        	else if(iUpgradeStatus == 3)
        	{
//			printf("网络连接失败,结果界面显示\n");
			gdk_threads_enter();
			gtk_widget_hide(updating_box);
	    		gtk_label_set_text(update_net_result_label,_("network problems,upgrade fail"));
			memset(image_temp,0,sizeof(image_temp));
			snprintf(image_temp,sizeof(image_temp),_("%supdate_warn.png"),ANTIVIRUS_IMAGE_DIR);
            		gtk_image_set_from_file(update_fail_success_image,image_temp);
            		change_font_style(update_net_result_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");
			gtk_widget_show(update_result_box);
			gtk_widget_show(update_net_result_label);
			gdk_threads_leave();
            		break;
        	}
        	else if(iUpgradeStatus == 2)
        	{
//			printf("当前为最新版本，升级成功,结果界面显示\n");
			gdk_threads_enter();
			gtk_widget_hide(updating_box);
 	    		gtk_label_set_text(update_net_result_label,_("upgrade success"));
			memset(image_temp,0,sizeof(image_temp));
			snprintf(image_temp,sizeof(image_temp),_("%supdate_success.png"),ANTIVIRUS_IMAGE_DIR);
            		gtk_image_set_from_file(update_fail_success_image,image_temp);
            		change_font_style(update_net_result_label,"<span foreground=\"#000000\" font_desc='12'>%s</span>");
			gtk_widget_show(update_result_box);
			gtk_widget_show(update_net_result_label);
			gdk_threads_leave();
         		break;
        	}
        	else if(iUpgradeStatus == 4)
        	{
			gdk_threads_enter();
			gtk_widget_hide(updating_box);
	    		gtk_label_set_text(update_net_result_label,_("engine is in use ,upgrade fail"));
			memset(image_temp,0,sizeof(image_temp));
			snprintf(image_temp,sizeof(image_temp),_("%supdate_warn.png"),ANTIVIRUS_IMAGE_DIR);
            		gtk_image_set_from_file(update_fail_success_image,image_temp);
            		change_font_style(update_net_result_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");

			gtk_widget_show(update_result_box);
			gtk_widget_show(update_net_result_label);
			gdk_threads_leave();
            		break;
        	}

		else if(iUpgradeStatus == 7 || iUpgradeStatus == 20)
		{
		        gdk_threads_enter();
                        gtk_widget_hide(updating_box);
                        gtk_label_set_text(update_net_result_label,_("network problems,upgrade fail"));
                        memset(image_temp,0,sizeof(image_temp));
                        snprintf(image_temp,sizeof(image_temp),_("%supdate_warn.png"),ANTIVIRUS_IMAGE_DIR);
                        gtk_image_set_from_file(update_fail_success_image,image_temp);
                        change_font_style(update_net_result_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");
                        gtk_widget_show(update_result_box);
                        gtk_widget_show(update_net_result_label);
                        gdk_threads_leave();
                        break;

		}
                else if(iUpgradeStatus == 6 || iUpgradeStatus == 8)
                {
		
                       if (strcmp(anti_engine_high_ver,anti_engine_ver_now)==0)
                       {
                           clamav_version_new = 1;
                           if (strcmp(engine_db_ver_now,apt_engin_db_ver)==0) 
                           {
                           	if(clamav_version_new&&clamav_thread_new)
                                {
					printf("无需升级\n");
					write_antivirus_status(status_temp,"1");
				}
			        else
                                {
				        printf("升级成功\n");
					write_antivirus_status(update_step_status_file,"2");
				       // write_antivirus_status(status_temp,"2");
			        }
                           }
                            
		      }
		      else
		      {
		          printf("无需升级\n");
			//  write_antivirus_status(status_temp,"1");
			  write_antivirus_status(update_step_status_file,"1"); 

		      }
                       
                      
                }
                else if(iUpgradeStatus == 14)
                {
                    if (strcmp(anti_engine_ver,anti_engine_high_ver)==0)
                    {
                            write_antivirus_status(status_temp,"25");
                    }

                }
		else if(iUpgradeStatus == 16)
		{
                      printf("3.2 病毒库版本相，显示label\n");
                       
                        memset(engine_db_server_ver,0,sizeof(engine_db_server_ver));
                        cmd_shell_result("dpkg -s cdos-virus | grep Description|cut -d ':' -f2 |sed s/[[:space:]]//g",engine_db_server_ver,sizeof(engine_db_server_ver));
                        engine_db_server_ver[strlen(engine_db_server_ver)-1]=0;


                        memset(cvd_version_result,0,sizeof(cvd_version_result));


                        if (strstr(gtk_label_get_text (clamav_thread_now_version),_("no virus database"))!=NULL)
                        {
                                run_check_sec_label=0;
                        }

                        else if (strstr(gtk_label_get_text (clamav_thread_now_version),_("upgrade virus library"))!=NULL)
                        {
                           
                                run_check_sec_label=0;
                        }
                        else
                        {
                                clamav_thread_new = 1;
                                run_check_sec_label=1;
                        }
                        if (run_check_sec_label==1)
                        {
                                engin_is_new_now=1;
                                write_antivirus_status(status_temp,"19");
                        }
                        else
                        {
                                write_antivirus_status(status_temp,"17");
                        }
                        gdk_threads_leave();


		    if(clamav_version_new&&clamav_thread_new)
		    {
			   printf("无需升级\n");
			   write_antivirus_status(status_temp,"1");
		    }
		    else
		    {
			printf("升级成功\n");
			write_antivirus_status(status_temp,"2");
		    }

		}
                else if (iUpgradeStatus == 18)
		{
			
			write_antivirus_status(status_temp,"19");
			
		}
               #endif

		/*else if(iUpgradeStatus == 6 || iUpgradeStatus == 7 ||  iUpgradeStatus == 8 ||  iUpgradeStatus ==9 || iUpgradeStatus == 10 || iUpgradeStatus == 11 || iUpgradeStatus == 12 || iUpgradeStatus == 13 || iUpgradeStatus == 14 || iUpgradeStatus == 15 || iUpgradeStatus == 16 || iUpgradeStatus == 17 || iUpgradeStatus == 18 || iUpgradeStatus == 19 || iUpgradeStatus == 20 || iUpgradeStatus == 21 || iUpgradeStatus == 22 || iUpgradeStatus == 23 || iUpgradeStatus == 24 || iUpgradeStatus == 25)
		{
			printf("iUpgradeStatus is :%d\n",iUpgradeStatus);
		       gdk_threads_enter();
		                               gtk_widget_hide(updating_box);
					                               gtk_label_set_text(update_net_result_label,_("upgrade success"));
								                               memset(image_temp,0,sizeof(image_temp));
											                               snprintf(image_temp,sizeof(image_temp),_("%supdate_success.png"),ANTIVIRUS_IMAGE_DIR);
														                               gtk_image_set_from_file(update_fail_success_image,image_temp);
																	                               change_font_style(update_net_result_label,"<span foreground=\"#000000\" font_desc='12'>%s</span>");
																				                               gtk_widget_show(update_result_box);
																							                               gtk_widget_show(update_net_result_label);
																										                               gdk_threads_leave();
																													                               break;

		}*/
		/*
		else if (iUpgradeStatus == 6)		//网络通过显示标签----“网络连接正常”
		{
			printf("1.1.1 网络连接正常显示，label显示\n");
			gdk_threads_enter();
			if(!gtk_widget_get_visible(update_net_botton))
			{
				gtk_widget_show(update_net_botton);
			}
//			printf("网络检测通过，显示label\n");
			gtk_label_set_text (updating_net_detail_label,_("net-is-ok"));
			gtk_widget_show(updating_net_detail_label);
			gtk_widget_show(updating_net_check_fix);
			write_antivirus_status(status_temp,"8");
			gdk_threads_leave();
		}
		else if (iUpgradeStatus == 7)		//网络不通显示标签----“网络连接失败”
		{
			printf("1. 网络连接失败显示，1-1 label显示\n");
			gdk_threads_enter();
			gtk_widget_show(update_net_botton);
			gtk_widget_show(updating_net_check_fix);
			gtk_label_set_text(updating_net_detail_label,_("net-is-fail"));
			gtk_widget_show(updating_net_detail_label);
	
			 write_antivirus_status(status_temp,"9");
			gdk_threads_leave();
		}
		else if (iUpgradeStatus == 8)		//网络通过显示标签-----“通过”
		{
			printf("1.1.2 网络检测通过，显示(通过)标签\n");
			gdk_threads_enter();
			gtk_label_set_text(updating_net_pass_label,_("pass"));
			change_font_style(updating_net_check_label,"<span foreground=\"#7cfc00\" font_desc='10'>%s</span>");
			change_font_style(updating_net_pass_label,"<span foreground=\"#7cfc00\" font_desc='10'>%s</span>");
			gtk_widget_show(updating_net_pass_label);
			 write_antivirus_status(status_temp,"24");
			gdk_threads_leave();
		}
		else if (iUpgradeStatus == 9)		//网络不通显示标签-----“失败”
		{
			printf("1. 网络检测失败，显示result *失败*\n");
			gdk_threads_enter();
			gtk_label_set_text(updating_net_pass_label,_("fail"));
			change_font_style(updating_net_pass_label,"<span foreground=\"#ff0000\" font_desc='10'>%s</span>");
			change_font_style(updating_net_check_label,"<span foreground=\"#ff0000\" font_desc='10'>%s</span>");
			gtk_widget_show(updating_net_pass_label);
			write_antivirus_status(status_temp,"3");
			gdk_threads_leave();
		}

		else if (iUpgradeStatus == 10)		//  版本相同，显示第一条标签----“当前版本为xxxx”
		{

			gdk_threads_enter();
			gtk_widget_show(clamav_button);
		//	sleep(1);
			gtk_widget_show(clamav_version_fix);	
			memset(anti_engine_check_label,0,sizeof(anti_engine_check_label));
//			snprintf(anti_engine_check_label,sizeof(anti_engine_check_label),("当前版本为%s，最高版本为%s"),anti_engine_ver_now,apt_clamav_ver);
			snprintf(anti_engine_check_label,sizeof(anti_engine_check_label),_("now version is %s，the highest version is %s"),anti_engine_ver_now,anti_engine_high_ver);
			gtk_label_set_text(clamav_now_version,anti_engine_check_label);
			if(clamav_now_version_label_status!=1)
			{
				gtk_widget_show(clamav_now_version);
			}
			write_antivirus_status(status_tem
;
			gdk_threads_leave();
//			sleep(1);
		}
		else if (iUpgradeStatus == 11)		// 版本相同，显示版本测试第二条----“当前版本为最新版本，无需升级”
		{
			gdk_threads_enter();
			gtk_label_set_text(clamav_update_success,_("now-is-new,no-update"));
			if(clamav_update_success!=1){
			gtk_widget_show(clamav_update_success);}
			write_antivirus_status(status_temp,"12");
			clamav_version_new = 1;
//			printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
//			printf("开始设置clamav第二个label%d\n",clamav_version_label_button_status);
			clamav_version_label_button_status = 1;
			gdk_threads_leave();
		}
		else if (iUpgradeStatus == 12)		// 版本检测标签显示是否通过-----显示“通过”
		{
			
			gdk_threads_enter();
			gtk_label_set_text(clamav_version_pass,_("pass"));
			change_font_style(clamav_version_pass,"<span foreground=\"#7cfc00\" font_desc='10'>%s</span>");
			change_font_style(clamav_version_check,"<span foreground=\"#7cfc00\" font_desc='10'>%s</span>");
			gtk_widget_show(clamav_version_pass);
			write_antivirus_status(status_temp,"25");
			gdk_threads_leave();
//			sleep(1);
		}
		else if (iUpgradeStatus == 13)		//clamav版本不同版本入口第一条label显示---“当前版本为xxx，最高版本为xxx，为您升级病毒库”
		{
			gdk_threads_enter();
			gtk_widget_show(clamav_button);
			
			gtk_widget_show(clamav_version_fix);	
			memset(anti_engine_check_label,0,sizeof(anti_engine_check_label));
//			snprintf(anti_engine_check_label,sizeof(anti_engine_check_label),"当前版本为%s,最高版本为%s，为您升级引擎",anti_engine_ver_now,anti_engine_high_ver);
			snprintf(anti_engine_check_label,sizeof(anti_engine_check_label),_("now version is %s,the highest version is %s，upgrade engine for you"),anti_engine_ver_now,anti_engine_high_ver);	
			gtk_label_set_text(clamav_now_version,anti_engine_check_label);
			gtk_widget_show(clamav_now_version);
//			sleep(5);
			printf("走这里了吗？\n");
			write_antivirus_status(update_step_status_file,"1");	//安装最新版本的clamav
			gdk_threads_leave();

	
		}
		else if (iUpgradeStatus == 14)		//判断显示升级clamav是否成功,clamav版本检测显示第二条
		{
			
			
			gdk_threads_enter();
			memset(anti_engine_ver,0,sizeof(anti_engine_ver));
			cmd_shell_result("bak=`dpkg -s cdos-antivirus | grep Version | cut -d ':' -f2 | cut -d '-' -f2`;version=1.0.$bak;echo $version",anti_engine_ver,512);
			anti_engine_ver[strlen(anti_engine_ver)-1]=0;
//			strncpy(anti_engine_ver,ENGINE_NOW_VER,sizeof(anti_engine_ver));
			memset(anti_engine_check_label,0,sizeof(anti_engine_check_label));
			if (strcmp(anti_engine_ver,anti_engine_high_ver)==0)
			{
//				snprintf(anti_engine_check_label,sizeof(anti_engine_check_label),"升级成功，当前杀毒引擎版本为%s",anti_engine_ver);
				snprintf(anti_engine_check_label,sizeof(anti_engine_check_label),_("Upgrade successfully,the current antivirus engine version is %s"),anti_engine_ver);
				printf("======================label值为%s=========================\n",anti_engine_check_label);
				gtk_label_set_text(clamav_update_success,anti_engine_check_label);
				gtk_widget_show(clamav_update_success);
				write_antivirus_status(status_temp,"12");
			}
			else
			{
//				snprintf(anti_engine_check_label,sizeof(anti_engine_check_label),"升级失败，当前杀毒引擎版本为%s,请检查网络与apt源",anti_engine_ver);
				snprintf(anti_engine_check_label,sizeof(anti_engine_check_label),_("Upgrade failed，the current antivirus engine version is %s,please check your network and repo source"),anti_engine_ver);
				printf("======================label值为%s=========================\n",anti_engine_check_label);
				gtk_label_set_text(clamav_update_success,anti_engine_check_label);
				gtk_widget_show(clamav_update_success);
				write_antivirus_status(status_temp,"15");
			}
			gdk_threads_leave();

		}
		else if (iUpgradeStatus == 15)		//版本检测显示“失败”
		{
			gdk_threads_enter();
			change_font_style(clamav_version_check,"<span foreground=\"#ff0000\" font_desc='10'>%s</span>");
			gtk_label_set_text(clamav_version_pass,_("fail"));
			change_font_style(clamav_version_pass,"<span foreground=\"#ff0000\" font_desc='10'>%s</span>");
                        gtk_widget_show(clamav_version_pass);
			write_antivirus_status(status_temp,"25");
			gdk_threads_leave();
		}

		else if (iUpgradeStatus == 16)
		{
			printf("3.2 病毒库版本相，显示label\n");
			gdk_threads_enter();
			memset(engine_db_server_ver,0,sizeof(engine_db_server_ver));
                        cmd_shell_result("dpkg -s cdos-virus | grep Description|cut -d ':' -f2 |sed s/[[:space:]]//g",engine_db_server_ver,sizeof(engine_db_server_ver));
                        engine_db_server_ver[strlen(engine_db_server_ver)-1]=0;
//			strncpy(engine_db_server_ver,ENGINE_LIB_NOW_VER,sizeof(engine_db_server_ver));

                        memset(cvd_version_result,0,sizeof(cvd_version_result));

//			if (strstr(gtk_label_get_text (clamav_thread_now_version),"暂无病毒库")!=NULL)
			if (strstr(gtk_label_get_text (clamav_thread_now_version),_("no virus database"))!=NULL)
			{
//				snprintf(cvd_version_result,sizeof(cvd_version_result),"病毒库安装完成,当前版本为%s",engine_db_server_ver);
				snprintf(cvd_version_result,sizeof(cvd_version_result),_("the virus database installation is complete,the current version is %s"),engine_db_server_ver);
				
                                gtk_label_set_text(clamav_thread_now_version,cvd_version_result);
                                run_check_sec_label=0;
			}
//			else if (strstr(gtk_label_get_text (clamav_thread_now_version),"为您升级病毒库")!=NULL)
			else if (strstr(gtk_label_get_text (clamav_thread_now_version),_("upgrade virus library"))!=NULL)
			{
//				snprintf(cvd_version_result,sizeof(cvd_version_result),"病毒库升级完成,当前版本为%s",engine_db_server_ver);
				snprintf(cvd_version_result,sizeof(cvd_version_result),_("virus database update complete,current version is %s"),engine_db_server_ver);
                                gtk_label_set_text(clamav_thread_now_version,cvd_version_result);
                                run_check_sec_label=0;
			}
			else
                        {
				printf("3.3 病毒库已为最新版本无需升级\n\n");
//                                snprintf(cvd_version_result,sizeof(cvd_version_result),"当前病毒库已为最新版本，无需升级",engine_db_server_ver);
                                snprintf(cvd_version_result,sizeof(cvd_version_result),_("the current virus database is newest version,no need update"),engine_db_server_ver);
                                gtk_label_set_text(clamav_thread_update_success_label,cvd_version_result);
                                gtk_widget_show(clamav_thread_update_success_label);
				clamav_thread_new = 1;
                                run_check_sec_label=1;
                        }
			if (run_check_sec_label==1)
			{
				engin_is_new_now=1;
                                write_antivirus_status(status_temp,"19");
			}
			else
			{
				write_antivirus_status(status_temp,"17");
			}
			gdk_threads_leave();
		}
		else if (iUpgradeStatus == 17)
		{
			printf("3.3 *当前不是最新版\n");
			gdk_threads_enter();
			memset(engine_db_ver,0,sizeof(engine_db_ver));
			snprintf(engine_db_ver,sizeof(engine_db_ver),_("update-success,now-version-is%s"),cvd_version_result);
			gtk_label_set_text(clamav_thread_update_success_label,engine_db_ver);
			if(clamav_thread_update_success_label_status!=1){
                        gtk_widget_show(clamav_thread_update_success_label);}
			write_antivirus_status(status_temp,"19");
			clamav_thread_label_button_status =1;
			gdk_threads_leave();
			
		}
		else if (iUpgradeStatus == 18)
		{
			printf("2. 当前是最新版，无需升级\n");
			gdk_threads_enter();
			gtk_label_set_text(clamav_thread_update_success_label,_("now-is-new,no-update"));
			clamav_thread_label_button_status = 1;
			clamav_thread_new = 1;
			if(clamav_thread_update_success_label_status!=1){
			gtk_widget_show(clamav_thread_update_success_label);}
			write_antivirus_status(status_temp,"19");
			gdk_threads_leave();
		}
		else if (iUpgradeStatus == 19)
		{
			printf("3.3 病毒库版本显示通过\n");
			gdk_threads_enter();
			gtk_label_set_text(updating_clamav_thread_pass_label,_("pass"));
			change_font_style(updating_clamav_thread_pass_label,"<span foreground=\"#7cfc00\" font_desc='10'>%s</span>");
			change_font_style(updating_clamav_thread_label,"<span foreground=\"#7cfc00\" font_desc='10'>%s</span>");
			gtk_widget_show(updating_clamav_thread_pass_label);
//			printf("clamav升级版本的值是%d,病毒库升级版本的值是%d",clamav_version_new,clamav_thread_new);
			if(clamav_version_new&&clamav_thread_new){
				printf("无需升级\n");
				write_antivirus_status(status_temp,"1");
			}
			else{
				printf("升级成功\n");
				write_antivirus_status(status_temp,"2");
			}
			gdk_threads_leave();
//			sleep(1);
		}
		else if (iUpgradeStatus == 20)
		{
			printf("3. 进入升级病毒库失败label显示！！！！！\n");
			gdk_threads_enter();
//			if (strstr(gtk_label_get_text (clamav_thread_now_version),"暂无病毒库")!=NULL)
			if (strstr(gtk_label_get_text (clamav_thread_now_version),_("no virus database"))!=NULL)
                        {
//                                gtk_label_set_text(clamav_thread_now_version,"病毒库安装失败");
                                gtk_label_set_text(clamav_thread_now_version,_("virus database install failed"));
				write_antivirus_status(status_temp,"21");
                        }
//                        if (strstr(gtk_label_get_text (clamav_thread_now_version),"为您升级病毒库")!=NULL)
                        if (strstr(gtk_label_get_text (clamav_thread_now_version),_("upgrade virus library"))!=NULL)
                        {
//                                gtk_label_set_text(clamav_thread_now_version,"病毒库升级失败");
                                gtk_label_set_text(clamav_thread_now_version,_("virus database update failed"));
				write_antivirus_status(status_temp,"21");
                        }

			else
                        {
                                memset(engine_db_server_ver,0,sizeof(engine_db_server_ver));

				strncpy(engine_db_server_ver,ENGINE_LIB_NOW_VER,sizeof(engine_db_server_ver));

                                memset(cvd_version_cmd,0,sizeof(cvd_version_cmd));
//                                snprintf(cvd_version_cmd,sizeof(cvd_version_cmd),"升级失败，当前版本为%s",engine_db_server_ver);
                                snprintf(cvd_version_cmd,sizeof(cvd_version_cmd),_("update failed ,current version is %s"),engine_db_server_ver);
                                gtk_label_set_text(clamav_thread_update_success_label,cvd_version_cmd);
                                gtk_widget_show(clamav_thread_update_success_label);
				gtk_label_set_text(updating_clamav_thread_pass_label,_("fail"));
                        	change_font_style(updating_clamav_thread_pass_label,"<span foreground=\"#ff0000\" font_desc='10'>%s</span>");
                        	change_font_style(updating_clamav_thread_label,"<span foreground=\"#ff0000\" font_desc='10'>%s</span>");
                        	gtk_widget_show(updating_clamav_thread_pass_label);
                                write_antivirus_status(status_temp,"3");
                        }	
			gdk_threads_leave();
		}
		else if (iUpgradeStatus == 21)
		{
			gdk_threads_enter();
			gtk_label_set_text(updating_clamav_thread_pass_label,_("fail"));
			change_font_style(updating_clamav_thread_pass_label,"<span foreground=\"#ff0000\" font_desc='10'>%s</span>");
			change_font_style(updating_clamav_thread_label,"<span foreground=\"#ff0000\" font_desc='10'>%s</span>");
			gtk_widget_show(updating_clamav_thread_pass_label);
			write_antivirus_status(status_temp,"22");	
			gdk_threads_leave();
		}
		 else if (iUpgradeStatus == 22)
                {
			gdk_threads_enter();
                        gtk_widget_hide(updating_box);

                        memset(engine_db_ver,0,sizeof(engine_db_ver));
                        snprintf(engine_db_ver, sizeof(engine_db_ver), "tail -1 %s",anti_upgrade_info_txt);
                        memset(cvd_version_result,0,sizeof(cvd_version_result));
                        cmd_shell_result(engine_db_ver, cvd_version_result, sizeof(cvd_version_result));
			if(strlen(cvd_version_result)==181){
				if(!gtk_widget_get_visible(update_net_result_label)){
					gtk_widget_show(update_net_result_label);
				}
			}
			else
			{
				if(!gtk_widget_get_visible(update_pro_result_label))
				{
                                        gtk_widget_show(update_pro_result_label);
                                }
			}
                        memset(image_temp,0,sizeof(image_temp));
                        snprintf(image_temp,sizeof(image_temp),_("%supdate_warn.png"),ANTIVIRUS_IMAGE_DIR);
                        gtk_image_set_from_file(update_fail_success_image,image_temp);
			gtk_widget_show(update_fail_success_image);
                        gtk_widget_show(update_result_box);
			gdk_threads_leave();
                        break;
                }
		else if (iUpgradeStatus == 23)          //安装新版本的
                {

			gdk_threads_enter();
                        memset(cvd_version_result,0,sizeof(cvd_version_result));


			char exit_nfsvirus_flag[512];
			memset(exit_nfsvirus_flag,0,sizeof(exit_nfsvirus_flag));
			cmd_shell_result("dpkg -l cdos-virus |wc -l",exit_nfsvirus_flag,512);
			exit_nfsvirus_flag[strlen(exit_nfsvirus_flag)-1]='\0';
						

//			if (strstr(engine_db_ver_now,"ClamAV")!=NULL)
			if (strcmp(exit_nfsvirus_flag,"0")==0)
                        {
				n++;
				if (n%2==0&&n!=0)
				{
//                                       	gtk_label_set_text(clamav_thread_now_version,"暂无病毒库,安装病毒库");
					gtk_label_set_text(clamav_thread_now_version,_("no virus database,installing the virus library"));
				}
				else if (n%2==1&&n!=0)
				{
//					gtk_label_set_text(clamav_thread_now_version,"暂无病毒库,安装病毒库...");
					gtk_label_set_text(clamav_thread_now_version,_("no virus database,installing the virus library..."));
				}
                 	}
                        else
                        {
				m++;
				if (m%2==0&&m!=0)
				{
//                                       	snprintf(cvd_version_result,sizeof(cvd_version_result),"当前病毒库版本为%s,为您升级病毒库",engine_db_ver_now);
					snprintf(cvd_version_result,sizeof(cvd_version_result),_("the current antivirus version is %s,upgrade virus library"),engine_db_ver_now);
				}
				else if (m%2==1&&m!=0)
				{
//                                       	snprintf(cvd_version_result,sizeof(cvd_version_result),"当前病毒库版本为%s,为您升级病毒库...",engine_db_ver_now);
					snprintf(cvd_version_result,sizeof(cvd_version_result),_("the current antivirus version is %s,upgrade virus library..."),engine_db_ver_now);
				}
//                                printf("显示label为%s\n",cvd_version_result);
                                gtk_label_set_text(clamav_thread_now_version,cvd_version_result);
             		}
                        gtk_widget_show(update_clamav_thread_fix);
//			printf("显示button标志位为%d\n",clamav_thread_now_version_label_status);
			if(clamav_thread_now_version_label_status!=1){
                        gtk_widget_show(clamav_thread_now_version);
                        gtk_widget_show(clamav_thread_button);
			}

			gdk_threads_leave();
                }
		else if (iUpgradeStatus == 24)          //clamav版本检测,检测当前版本是否为最新
		{
			printf("二、 杀毒软件版本检测开始\n");
			printf("注意：当前杀毒软件版本为%s,apt源杀毒软件版本为%s\n",anti_engine_ver_now,anti_engine_high_ver);
			gdk_threads_enter();
			if (strcmp(anti_engine_high_ver,anti_engine_ver_now)==0)
			{
				printf("2.1.1 杀毒软件版本相同\n");
        			write_antivirus_status(status_temp,"10");
			}
			else
			{
				printf("2.1.1 杀毒软件版本不同\n");
			        write_antivirus_status(status_temp,"13");
			}
			gdk_threads_leave();
		}
//dsj-0123
		else if (iUpgradeStatus == 25)          //显示病毒库当前版本
                {
			gdk_threads_enter();
                 //       if (Check_Net_Connect() == -1)
                        if (checkConnect("www.baidu.com",4) == -1)
                        {
                        	printf("3.1.1 网络检测，连接失败\n");
                                gtk_label_set_text(clamav_thread_now_version,"网络连接失败，请检查网络");
                                gtk_widget_show(clamav_thread_now_version);
                                gtk_widget_show(clamav_thread_button);
                                gtk_widget_show(update_clamav_thread_fix);
                                write_antivirus_status(status_temp,"20");

                        }
                        else
                        {
				printf("3.1.1 网络检测，连接成功\n");
//                                memset(engine_db_server_ver,0,sizeof(engine_db_server_ver));
//				strncpy(engine_db_server_ver,ENGINE_LIB_HIGH_VER,sizeof(engine_db_server_ver));

                                if (strcmp(engine_db_ver_now,apt_engin_db_ver)==0)  //当前已为最新版本
                                {
					printf("**病毒库版本相同,病毒库版本为%s\n",engine_db_ver_now);
                                        memset(cvd_version_result,0,sizeof(cvd_version_result));
//                                        snprintf(cvd_version_result,sizeof(cvd_version_result),"当前版本为%s,最高版本为%s",engine_db_ver_now,engine_db_server_ver);
                                        snprintf(cvd_version_result,sizeof(cvd_version_result),_("current version is %s,the newest version is %s"),engine_db_ver_now,apt_engin_db_ver);
//                                        printf("版本相同,版本号w为 %s\n",engine_db_ver_now);
                                        gtk_label_set_text(clamav_thread_now_version,cvd_version_result);
                                        gtk_widget_show(clamav_thread_now_version);
                                        gtk_widget_show(clamav_thread_button);
                                        gtk_widget_show(update_clamav_thread_fix);
                                        write_antivirus_status(status_temp,"16");
                                }
                                else
                                {
	
                        		write_antivirus_status(update_step_status_file,"2");    //安装新版本的
                                        write_antivirus_status(status_temp,"23");
                                }
                        }
                }
		gdk_threads_leave();*/
    	}
	gdk_threads_enter();
	int avlStatus = 0;
    	char upgrade_dir[512];
        snprintf(upgrade_dir,sizeof(upgrade_dir),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    	avlStatus = get_upgrde_status(upgrade_dir);
    	if(avlInit == 0 || avlStatus == 6 )
	{
        	printf("初始化没有完毕\n");

    	}
	else
	{
#if 0
        	char engin_ver[1024];
        	cmd_shell_result("dpkg -s cdos-virus | grep Description|cut -d ':' -f2 |sed s/[[:space:]]//g",engin_ver,1024);
        	engin_ver[strlen(engin_ver)-1]=0;
//		strncpy(engin_ver,ENGINE_MAIN_PAGE_VER,sizeof(engin_ver));
		printf("？？？？？？？病毒库版本为%s\n",engin_ver);	

        	char engin_title[1024];
        	snprintf(engin_title,sizeof(engin_title),_("Engine version:%s"),engin_ver);
        	gtk_label_set_text(attentionlabel,engin_title);
        	change_font_style((GtkLabel *)attentionlabel,"<span foreground=\"#74787c\" font_desc='10'>%s</span>");
#endif
    	}
#if  0
	char *old_last_update_label_value = gtk_label_get_text(last_update_label);
	char old_year[10];
	char old_month[5];
	char old_day[5];
	old_year[0]=old_last_update_label_value[27];
	old_year[1]=old_last_update_label_value[28];
	old_year[2]=old_last_update_label_value[29];
	old_year[3]=old_last_update_label_value[30];
	old_year[4]='\0';
	old_month[0]=old_last_update_label_value[34];
	old_month[1]=old_last_update_label_value[35];
	old_month[2]='\0';
	old_day[0]= old_last_update_label_value[39];
	old_day[1]=old_last_update_label_value[40];
	printf("旧的日是%s\n",old_day);
	

	
	char last_update_time[200];
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
#endif
	clamav_thread_new = 0;
	clamav_version_new = 0;	
	int update_flag= get_upgrde_status(status_temp);
	
#if 0
	if(strcmp(old_year,year_month_day[0])==0&&strcmp(old_month,year_month_day[1])==0&&strcmp(old_day,year_month_day[2])==0){
	}
	else{
		if (update_flag != 1 || strcmp(old_year,"")==0)
		{
        		//sprintf(last_update_time,_("you last update is:%syear,%smonth,%sday"),year_month_day[0],year_month_day[1],year_month_day[2]);
        		//gtk_label_set_text(last_update_label,last_update_time);
        		//change_font_style((GtkLabel *)last_update_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");
		}
	}
#endif
	gdk_threads_leave();
}

int get_current_int_antivirus_version()
{
    char anti_engine_ver_update_now[512];
    cmd_shell_result("bak=`dpkg -s cdos-antivirus | grep Version | cut -d ':' -f2 | cut -d '-' -f2`;version=1.0.$bak;echo $version",anti_engine_ver_update_now,512);
    anti_engine_ver_update_now[strlen(anti_engine_ver_update_now)-1]=0;
    printf("升级后杀毒软件版本号为%s\n",anti_engine_ver_update_now);

    int iUpdateAntiVer = 0;
    if(strlen(anti_engine_ver_update_now) > 0)
    {
	replace_string(anti_engine_ver_update_now,".","");
	iUpdateAntiVer = atoi(anti_engine_ver_update_now);
	printf("shuzi %d\n",atoi(anti_engine_ver_update_now));
    }
    return iUpdateAntiVer;

}

int get_current_apt_antivirus_version()
{
   int iAptAntiVer;
   char anti_engine_high_ver[512];
   cmd_shell_result("ver_temp=`apt-cache show cdos-antivirus | grep Version |head -n 1 | cut -d '-' -f2`;ver=1.0.$ver_temp;echo $ver",anti_engine_high_ver,512);
   anti_engine_high_ver[strlen(anti_engine_high_ver)-1]=0;
   printf("当前系统源中杀毒软件版本为%s\n",anti_engine_high_ver);

   if(strlen(anti_engine_high_ver) > 0)
   {
	replace_string(anti_engine_high_ver,".","");

	iAptAntiVer = atoi(anti_engine_high_ver);
	printf("shuzi high %d\n",atoi(anti_engine_high_ver));
   }
   return iAptAntiVer;


}
int get_current_int_engine_version()
{
    int iCurVirusVer=0 ;
    char sys_virus_ver[512];
    memset(sys_virus_ver,0,sizeof(sys_virus_ver));
    cmd_shell_result("bak=`dpkg -s cdos-virus | grep Version | cut -d ':' -f2 | cut -d '-' -f2`;version=$bak;echo $version",sys_virus_ver,sizeof(sys_virus_ver));
    sys_virus_ver[strlen(sys_virus_ver)-1]='\0';
    if(strlen(sys_virus_ver) > 0 )
    {
	iCurVirusVer=atoi(sys_virus_ver);
	printf("sys_virus_ver is :%s\n",sys_virus_ver);
	printf("iCurVirusVer is :%d\n",iCurVirusVer);
    }
    return iCurVirusVer;
}
int get_current_apt_engine_version()
{
   int iAptVirusVer = 0;

/**
	 *   *获取apt源软件版本--1.0.1
	 *   **/
        char apt_virus_db_ver[512];
	memset(apt_virus_db_ver,0,sizeof(apt_virus_db_ver));
	cmd_shell_result("ver_temp=`apt-cache show cdos-virus | grep Version |head -n 1 | cut -d '-' -f2|cut -d '+' -f1 `;ver=$ver_temp;echo $ver",apt_virus_db_ver,sizeof(apt_virus_db_ver));
	apt_virus_db_ver[strlen(apt_virus_db_ver)-1]='\0';
				//
	printf("apt_virus_db_ver is :%s\n",apt_virus_db_ver);
	if(strlen(apt_virus_db_ver) > 0 )
        {
	    iAptVirusVer=atoi(apt_virus_db_ver);
	    printf("iAptVirusVer is :%d",iAptVirusVer);
	    
	}
    return iAptVirusVer;

}

/***

****/
int update_engin_version(int iAptVirusVer)
{

    char cmd_temp[512]; 
    memset(cmd_temp,0,sizeof(cmd_temp));
    cmd_shell_result("apt-get --reinstall install cdos-virus --assume-yes --force-yes  >/dev/null 2>&1;echo $?",cmd_temp,512);
    cmd_temp[strlen(cmd_temp)-1]=0;

    int iCurVirusVer = get_current_int_engine_version();
    if(iCurVirusVer == iAptVirusVer)
    {
        return 1;

			     
   }
   else
   {
	return -1;

   }
			    
}

int update_antivirus_version(int iAptAntiVer)
{
    char cmd_temp[512];
    memset(cmd_temp,0,sizeof(cmd_temp));
    cmd_shell_result("apt-get --reinstall install cdos-antivirus --assume-yes --force-yes  >/dev/null 2>&1;echo $?",cmd_temp,512);
    cmd_temp[strlen(cmd_temp)-1]=0;
    printf("cmd_temp is :%s\n",cmd_temp);

		
    memset(cmd_temp,0,sizeof(cmd_temp));
    cmd_shell_result("apt-get --reinstall install cdos-antivirus-base --assume-yes --force-yes  >/dev/null 2>&1;echo $?",cmd_temp,512);
    cmd_temp[strlen(cmd_temp)-1]=0;

    memset(cmd_temp,0,sizeof(cmd_temp));
    cmd_shell_result("apt-get --reinstall install cdos-antivirus-daemon --assume-yes --force-yes  >/dev/null 2>&1;echo $?",cmd_temp,512);
    cmd_temp[strlen(cmd_temp)-1]=0;
			
    memset(cmd_temp,0,sizeof(cmd_temp));
    cmd_shell_result("apt-get --reinstall install cdos-antivirus-lib6 --assume-yes --force-yes  >/dev/null 2>&1;echo $?",cmd_temp,512);
    cmd_temp[strlen(cmd_temp)-1]=0;
			
    sleep(10);
    int iUpdateAntiVer = get_current_int_antivirus_version();
    if(iUpdateAntiVer == iAptAntiVer)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

void  file_modify_time(int * size , char *file_name)
{

	FILE * fp;
	int fd;
	struct stat buf;
	if((access(file_name,F_OK)) ==-1)
	{
	     *size = 0;
	     return ;
	}

	fp=fopen(file_name,"r");
	fd=fileno(fp);
	fstat(fd, &buf);
	*size = buf.st_size; //get file size (byte)
	long modify_time=buf.st_mtime; //latest modification time (seconds passed from 01/01/00:00:00 1970 UTC)
	printf("file size=%d \n",*size);
	fclose(fp);


}


void avlUpgradeLibRun()
{
 //   	startUpgrade();
	printf("&&&&&&&&&&&&&&&&&&&&&&&&&&()()进入升级的另外一个线程()()&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
	startUpgrade();
        char update_step_status_file[512];
        snprintf(update_step_status_file,sizeof(update_step_status_file),"%savlUpgradeStartStatus.txt",ANTIVIRUS_TEMP_DIR);
	char status_temp[1024];
        snprintf(status_temp,sizeof(status_temp),"%savlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR);	
	char update_anti_db_result_file[512];
	snprintf(update_anti_db_result_file,sizeof(update_anti_db_result_file),"%supgradeInfo.txt",ANTIVIRUS_TEMP_DIR);

        int avl_update_step_status=0;
        char cmd_temp[512];
	int n=0;

        int iAptVirusVer = 0, iAptAntiVer = 0 , iCurVirusVer = 0, iCurAntVer = 0;

	int old_size = 0;
	long old_modify_time = 0;
	file_modify_time(&old_size , "/var/lib/clamav/bytecode.cvd");

	char upgradeCmd[256];
	FILE * upgradeId;
	char update_database_cmd[256] = {0};
	char tempInfo[256] = {0};

	sprintf(upgradeCmd, " freshclam > %supgradeInfo.txt 2>&1",ANTIVIRUS_TEMP_DIR);
	upgradeId = popen(upgradeCmd, "r");
	pclose(upgradeId);

	int new_size = 0;
	long new_modify_time = 0;
	file_modify_time(&new_size ,"/var/lib/clamav/bytecode.cvd");

#if 0
	iAptVirusVer = get_current_apt_engine_version();
        iCurVirusVer = get_current_int_engine_version();
        if(iAptVirusVer > iCurVirusVer )
        {
            iUpdateEngineFlag = update_engin_version(iAptVirusVer);
        }
#endif 
	iUpdateEngineFlag = 0;

	if(new_size >= old_size)
	{
	     int source_size = 0;
	     file_modify_time(&source_size ,"/var/lib/nfs-virus/bytecode.cvd");
	     if(new_size > source_size)
	     {
		iUpdateEngineFlag = 1;

		strcpy(update_database_cmd,"cp /var/lib/clamav/* /var/lib/nfs-virus/");
		upgradeId = popen(update_database_cmd, "r");
		fread(tempInfo, sizeof(char), sizeof(tempInfo)-1, upgradeId);
		pclose(upgradeId);
	     }

	
	}


        iAptAntiVer = get_current_apt_antivirus_version();
        iCurAntVer = get_current_int_antivirus_version();
        if(iAptAntiVer > iCurAntVer)
        {
            iUpdateAntivirusFlag = update_antivirus_version(iAptAntiVer);
        }
        
        sleep(5);
       
        if( iUpdateEngineFlag == 1)
	{
		write_antivirus_status(status_temp,"2");
	}
	else
	{
		write_antivirus_status(status_temp,"1");
	}


	printf("write over\n");

	return;

}

