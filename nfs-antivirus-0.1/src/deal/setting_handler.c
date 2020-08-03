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
#include <gtk/gtk.h>
#include <locale.h>
#include <libintl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/stat.h>
#include "../page/setting-page.h"
#include "../deal/setting_handler.h"
#include "../antivirus.h"
#include "home_handle.h"
#include "../function.h"
#define _(STRING) gettext(STRING)

#define MONTHLY_ANTIVIRUS  1
#define WEEKLY_ANTIVIRUS   2
#define DAYLY_ANTIVIRUS    3
extern char scan_starttime[30];
char devide_file[256];
extern int auto_timing_anti_flag;
extern GtkWidget *path_entry,*real_time_prtect_button,*real_time_protect_dir,*choose_protect_button,*real_time_protect_is_label;;
extern GtkWidget *devide_file_dir,*window,*devide_content_box,*common_settings_fix,*common_setting_content_box,*setbutton;
extern int area;
extern char g_current_user[256]; //当前启动杀毒软件用户
int select_protect_dir(GtkWidget *widget,gpointer *data)
{
	GtkWidget *select_protect_dir_filechooser;
   	char protect_dir_info[256];
	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"/usr/share/antivirus/config/%s_setting.txt",g_current_user);
   	select_protect_dir_filechooser =  gtk_file_chooser_dialog_new (_("File selection"),GTK_WINDOW(window),
                                                GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                GTK_STOCK_CANCEL,
                                                GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_OK,
                                                GTK_RESPONSE_OK,
                                                NULL);

   	if(gtk_dialog_run (GTK_DIALOG (select_protect_dir_filechooser)) == GTK_RESPONSE_OK)
   	{
         	gtk_widget_hide(select_protect_dir_filechooser);
       		strcpy(protect_dir_info,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(select_protect_dir_filechooser)));
        	set_antivirus_setting_item("real-time-protect",protect_dir_info,set_file_path);
        	gtk_label_set_text(data,protect_dir_info);
        }
   	gtk_widget_destroy(select_protect_dir_filechooser);
    	return 0;
}
int check_start_real_protect()
{
	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"/usr/share/antivirus/config/%s_setting.txt",g_current_user);
		    
	int check_protect = get_antivirus_setting_item_inttype("check-real-time-protect",set_file_path);
        char cmd[1024] = "ps aux|grep /usr/sbin/antivirus-d|grep -v grep";
        FILE *ptr;
	char cmd_result[1024];
        if((ptr = popen(cmd, "r")) != NULL)
	{
		if(fgets(cmd_result,1024,ptr)!=NULL)
		{	
			fclose(ptr);
			if(strcmp(cmd_result,"") != 0)
			{
                		if(check_protect==1)
				{
                        		return 1;
                		}
                		else
				{
                        		return 2;
                		}
			}
			else
			{
				return 0;
			}
		}
		else
		{
                        return 0;
		}
        }
        else
	{
                return 0;
        }
}

int select_devide_file(GtkWidget *widget,gpointer *data)
{
   	GtkWidget *select_devide_dir_filechooser;
	char devidefile_dir_info[256];
	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);


   	select_devide_dir_filechooser =  gtk_file_chooser_dialog_new (_("File selection"),GTK_WINDOW(window),
                                                GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                GTK_STOCK_CANCEL,
                                                GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_OK,
                                                GTK_RESPONSE_OK,
                                                NULL);

   	if(gtk_dialog_run (GTK_DIALOG (select_devide_dir_filechooser)) == GTK_RESPONSE_OK)
   	{
         	gtk_widget_hide(select_devide_dir_filechooser);
       		strcpy(devidefile_dir_info,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(select_devide_dir_filechooser)));
        	set_antivirus_setting_item("devide-dir",devidefile_dir_info,set_file_path);
		if(strlen(devidefile_dir_info)>30)
		{
			char devidefile_dir_show_info[40];
        		strncpy(devidefile_dir_show_info,devidefile_dir_info, 30);
        		int num_hanzi=0;
        		int i=0;
        		while(i<30)
			{
                		if(devidefile_dir_info[i]&0x80)
				{
                        		num_hanzi++;
                		}
                		i++;
        		}
        		if(num_hanzi%3==1)
			{
                		devidefile_dir_show_info[30]=devidefile_dir_info[30];
                		devidefile_dir_show_info[31]=devidefile_dir_info[31];
                		devidefile_dir_show_info[32]='.';
                		devidefile_dir_show_info[33]='.';
                		devidefile_dir_show_info[34]='.';
                		devidefile_dir_show_info[35]='\0';
        		}
        		else if(num_hanzi%3==2)
			{
                		devidefile_dir_show_info[30]=devidefile_dir_info[30];
                		devidefile_dir_show_info[31]='.';
                		devidefile_dir_show_info[32]='.';
                		devidefile_dir_show_info[33]='.';
                		devidefile_dir_show_info[34]='\0';
        		}
        		else 
			{
                		devidefile_dir_show_info[30]='.';
                		devidefile_dir_show_info[31]='.';
                		devidefile_dir_show_info[32]='.';
                		devidefile_dir_show_info[33]='\0';
        		}
        		gtk_label_set_text(devide_file_dir,devidefile_dir_show_info);
        	}
		else
		{
        		gtk_label_set_text(devide_file_dir,devidefile_dir_info);
		}
	 	gtk_widget_set_tooltip_text(devide_file_dir,devidefile_dir_info);
        }
	
   	gtk_widget_destroy(select_devide_dir_filechooser);
    	return 0;
}

int get_week_num(gchar * c_week_num)
{
	if(strcmp(c_week_num,_("MON")) == 0)
    	{
        	return 0;
    	}
    	else if(strcmp(c_week_num,_("TUE")) == 0)
    	{
        	return 1;
    	}
    	else if(strcmp(c_week_num,_("WED")) == 0)
    	{
        	return 2;
    	}
    	else if(strcmp(c_week_num,_("THU")) == 0)
    	{
        	return 3;
    	}
    	else if(strcmp(c_week_num,_("FRI")) == 0)
    	{
        	return 4;
    	}
    	else if(strcmp(c_week_num,_("SAT")) == 0)
    	{
        	return 5;
    	}
    	else if(strcmp(c_week_num,_("SUN")) == 0)
    	{
        	return 6;
    	}
    	return -1;
}

void set_time_anti_backstage(char *scan_anti_style,int time_anti_type, char *anti_time1, char *anti_time2, char *anti_time3)
{
    	char crontabCmd[1024];
    	system("sed -i '/timeantivirus/d'  /var/spool/cron/root 2>&1 ");
	char time_antivirus_path[50];
	snprintf(time_antivirus_path,sizeof(time_antivirus_path),"%s.%s_time_antivirus.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
    	char cmd[512];
    	sprintf(cmd,"rm -rf %s",time_antivirus_path);
    	system(cmd);
    	if(strcmp(scan_anti_style, "快速扫描")== 0)
    	{
        	if( time_anti_type == MONTHLY_ANTIVIRUS)
        	{
                	snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s %s * * sh %squick_timeantivirus.sh %s \" >> /tmp/setcron.txt 2>&1 ",anti_time3,anti_time2,anti_time1,ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR,g_current_user);
        	}
        	else if( time_anti_type == WEEKLY_ANTIVIRUS )
        	{
                	int week_num = atoi(anti_time1) + 1;
        //        	snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s * * %d sh %squick_timeantivirus.sh %s \" >> /var/spool/cron/root 2>&1 ",anti_time3,anti_time2,week_num,ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR,g_current_user);
			snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s * * %d sh %squick_timeantivirus.sh %s \" >> /tmp/setcron.txt 2>&1 ",anti_time3,anti_time2,week_num,ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR,g_current_user);
        	}
        	else if( time_anti_type == DAYLY_ANTIVIRUS )
        	{
                	snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s * * * sh %squick_timeantivirus.sh %s \" >> /tmp/setcron.txt 2>&1 ",anti_time2,anti_time1,ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR,g_current_user);
        	}
    	}
    	else if (strcmp(scan_anti_style , "全盘扫描") == 0)
    	{
        	if( time_anti_type == MONTHLY_ANTIVIRUS)
        	{
                	snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s %s * * sh %sall_timeantivirus.sh %s \" >> /temp/setcron.txt 2>&1 ",anti_time3,anti_time2,anti_time1,ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR,g_current_user);
        	}
        	else if( time_anti_type == WEEKLY_ANTIVIRUS )
        	{
                	int week_num = atoi(anti_time1) + 1;
                	snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s * * %d sh %sall_timeantivirus.sh %s \" >> /tmp/setcron.txt 2>&1 ",anti_time3,anti_time2,week_num,ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR,g_current_user);
        	}
        	else if( time_anti_type == DAYLY_ANTIVIRUS )
        	{
                	snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s * * * sh %sall_timeantivirus.sh  %s \" >> /tmp/setcron.txt 2>&1 ",anti_time2,anti_time1,ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR,g_current_user);
        	}

    	}
    	system(crontabCmd);
	memset(cmd,0,sizeof(cmd));
	snprintf(cmd,sizeof(cmd),"systemctl restart antivirusCron.service");
    	system(cmd);

	system("rm -rf /tmp/setcron.txt");

}

void set_scan_style(gchar *scan_anti_style)
{
	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);



	if(strcmp(scan_anti_style,_("all-scan")) == 0)
	{
		set_antivirus_setting_item("timing-scan-style","0",set_file_path);

        }
        else if(strcmp(scan_anti_style,_("quick-scan")) == 0)
	{
		set_antivirus_setting_item("timing-scan-style","1",set_file_path);
        }
        else if(strcmp(scan_anti_style,_("custom-scan")) == 0)
	{
		set_antivirus_setting_item("timing-scan-style","2",set_file_path);
        }
        return ;

}

void save_scan_anti(GtkWidget *widget,gpointer data)
{	
		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);


        int startTimeAntFlag = 0;//开启定时杀毒设置标志为0
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(manual_deal_button)))
	    {
                set_antivirus_setting_item("anti-deal-style","1",set_file_path);
        }
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(intelligence_deal_button)))
	    {
                set_antivirus_setting_item("anti-deal-style","2",set_file_path);
        }

        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(isolation_region_button)))
	    {
                set_antivirus_setting_item("file-into-deivide","1", set_file_path);

        }
        else
	    {
                set_antivirus_setting_item("file-into-deivide","0",set_file_path);
        }
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(timing_anti_button)))
        {
                set_antivirus_setting_item("start-timing-anti","1",set_file_path);
                startTimeAntFlag = 1;
        }
        else
	{
                set_antivirus_setting_item("start-timing-anti","0",set_file_path);
                startTimeAntFlag = 0;

		system("echo 1 >> /tmp/cancelscan.txt");

		char cmd[96];
		memset(cmd,0,sizeof(cmd));
                snprintf(cmd,sizeof(cmd),"systemctl restart antivirusCron.service");
                system(cmd);

        }
        gchar *scan_anti_style = gtk_combo_box_text_get_active_text((GtkComboBoxText *)anti_scan_combo);
        set_scan_style(scan_anti_style);
        gchar *anti_time1;
        gchar *anti_time2;
        gchar *anti_time3;
        if(auto_timing_anti_flag==1)
        {
                set_antivirus_setting_item("timing-scan-select","1",set_file_path);
        }
        char each_month_setting[1024];

        anti_time1 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(month_anti_day_combo));
        anti_time2 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(month_anti_hour_combo));
        anti_time3 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(month_anti_min_combo));
        if(auto_timing_anti_flag==1 && startTimeAntFlag==1)
        {
           set_time_anti_backstage(scan_anti_style,MONTHLY_ANTIVIRUS,anti_time1,anti_time2,anti_time3);
 
        }

        strcat_more_string(each_month_setting,1024,anti_time1,anti_time2,anti_time3);
        set_antivirus_setting_item("timing-scan-month",each_month_setting,set_file_path);
        if(auto_timing_anti_flag==2)
        {
                set_antivirus_setting_item("timing-scan-select","2",set_file_path);
        }
        char each_week_setting[1024];
        anti_time1 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT( week_anit_week_combo));
        int week_num = 0;
        week_num = get_week_num(anti_time1);
        snprintf(anti_time1,sizeof(anti_time1),"%d",week_num) ;
        anti_time2 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(week_anit_hour_combo));
        anti_time3 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(week_anit_min_combo));
        if(auto_timing_anti_flag==2 && startTimeAntFlag==1)
        {
           set_time_anti_backstage(scan_anti_style,WEEKLY_ANTIVIRUS,anti_time1,anti_time2,anti_time3);
        }

        strcat_more_string(each_week_setting,1024,anti_time1,anti_time2,anti_time3);
        set_antivirus_setting_item("timing-scan-week",each_week_setting,set_file_path);
        if(auto_timing_anti_flag==3)
        {
                set_antivirus_setting_item("timing-scan-select","3",set_file_path);
        }
        char each_day_setting[1024];

        anti_time1 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(day_anti_hour_combo));
        anti_time2 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(day_anit_min_combo));
        anti_time3 = "15";

        strcat_more_string(each_day_setting,1024,anti_time1,anti_time2,anti_time3);
				set_antivirus_setting_item("timing-scan-day",each_day_setting,set_file_path);
				if(auto_timing_anti_flag==3 && startTimeAntFlag==1)
				{
				   set_time_anti_backstage(scan_anti_style,DAYLY_ANTIVIRUS,anti_time1,anti_time2,anti_time3);
				}
			show_warn_info(_("save sucess"),0);

		}

		void boot_start_deal(int start_status)
		{
				char cmd[256];

				if(start_status == BOOT_START_TRUE)
				{
					snprintf(cmd,sizeof(cmd),"sh %sboot_start.sh 1 ",ANTIVIRUS_BIN_AUTO_DEFENSE_DIR);
					system(cmd);
				}
				if(start_status == BOOT_START_FALSE)
				{
					snprintf(cmd,sizeof(cmd),"sh %sboot_start.sh 0 ",ANTIVIRUS_BIN_AUTO_DEFENSE_DIR);
					system(cmd);
				}

		}

		void save_common_settings(GtkWidget *widget,gpointer data)
		{
			char cmd[1024];
			char set_file_path[1024];
			snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);


			if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(boot_start_button)))
	{
		set_antivirus_setting_item("boot-start","1",set_file_path);
                //设置开机自启动
                boot_start_deal(BOOT_START_TRUE);
		char defence_log[1024];
        	//char *defence_log_type=_("boot-start");
		char defence_log_type[1024];
        	strcpy(defence_log_type,_("boot-start"));
        	char *defenc_log_result=_("open");
		getsystime();
		snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
		write_defence_log(defence_log);
        }
        else
	{
		set_antivirus_setting_item("boot-start","0",set_file_path);
                //设置取消开机自启动
                boot_start_deal(BOOT_START_FALSE);
		char defence_log[1024];
                //char *defence_log_type=_("boot-start");
		char defence_log_type[1024];
        	strcpy(defence_log_type,_("boot-start"));
                char *defenc_log_result=_("close");
                getsystime();
                snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
                write_defence_log(defence_log);
        }
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(aotu_usb_button)))
	{
		set_antivirus_setting_item("auto-check-mobile-dev","1",set_file_path);
        }
        else
	{
		set_antivirus_setting_item("auto-check-mobile-dev","0",set_file_path);
        }
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(download_button)))
	{
		set_antivirus_setting_item("auto-check-download","1",set_file_path);
        }
        else
	{
		set_antivirus_setting_item("auto-check-download","0",set_file_path);
        }
	int check = check_start_real_protect();
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(real_time_prtect_button)))
	{
		if( (check==0) || (check==2) )
		{
			memset(cmd,0,sizeof(cmd));
			snprintf(cmd,sizeof(cmd),"cp %santivirus-use.conf /etc/antivirus/antivirus.conf",ANTIVIRUS_CONF_DIR);
                        system(cmd);
                        clamav_daemon_operator();
                }
                set_antivirus_setting_item("check-real-time-protect","1",set_file_path);

        }
        else
	{
		if((check==0) || (check==1))
		{
			memset(cmd,0,sizeof(cmd));
			snprintf(cmd,sizeof(cmd),"cp %santivirus-un-use.conf /etc/antivirus/antivirus.conf",ANTIVIRUS_CONF_DIR);
                        system(cmd);
                        clamav_daemon_operator();
                }
                set_antivirus_setting_item("check-real-time-protect","0",set_file_path);
        }
	printf("in delete setcron.txt");
        system("rm -rf /tmp/setcron.txt");

	show_warn_info(_("save sucess"),0);
}
void save_update_set(GtkWidget *widget,gpointer data)
{
	gchar *update_month_day_value,*update_month_hour_value,*update_month_min_value,*update_week_week_value,*update_week_hour_value,*update_week_min_value,*update_day_hour_value,*update_day_min_value,*update_day_sec_value;
	char month_update_setting[1024];
	char week_update_setting[1024];
	char day_update_setting[1024];
	char crontabCmd[1024];
	char cmd[1024];
	
	int update_time_set;
	int update_set;


	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);

	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(timingbutton)))
	{
                update_set = 1;
                set_antivirus_setting_item("update-style","1", set_file_path);
             //   system("sed -i '/upgrade/d'  /var/spool/cron/root 2>&1 ");

		system("echo 1 >> /tmp/cancelupdate.txt");

	       system(crontabCmd);
               memset(cmd,0,sizeof(cmd));
               snprintf(cmd,sizeof(cmd),"systemctl restart antivirusCron.service");
	       system(cmd);


	       printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaxxxxxxxxxxxxxx\n");
	        show_warn_info(_("save sucess"),0);
                return;

        }

	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(periodbuton)))
	{
		update_set = 2;
                set_antivirus_setting_item("update-style","2", set_file_path);
	}
	
	update_month_day_value =gtk_combo_box_text_get_active_text(Combo_Box_month_day);
	update_month_hour_value = gtk_combo_box_text_get_active_text(Combo_Box_month_hour);
        update_month_min_value = gtk_combo_box_text_get_active_text(Combo_Box_month_min);
        strcat_more_string(month_update_setting,1024,update_month_day_value,update_month_hour_value,update_month_min_value);
        set_antivirus_setting_item("timing-update-month",month_update_setting,set_file_path);

	update_week_week_value = gtk_combo_box_text_get_active_text(Combo_Box_week_week);
        update_week_hour_value = gtk_combo_box_text_get_active_text(Combo_Box_week_hour);
        update_week_min_value = gtk_combo_box_text_get_active_text(Combo_Box_week_min);
        int week_num = 0;
        week_num = get_week_num(update_week_week_value);
        snprintf(update_week_week_value,sizeof(update_week_week_value),"%d",week_num) ;
        strcat_more_string(week_update_setting,1024,update_week_week_value,update_week_hour_value,update_week_min_value);
        set_antivirus_setting_item("timing-update-week",week_update_setting, set_file_path);

	update_day_hour_value = gtk_combo_box_text_get_active_text(Combo_Box_day_hour);
        update_day_min_value = gtk_combo_box_text_get_active_text(Combo_Box_day_min);
        update_day_sec_value = "15";
        strcat_more_string(day_update_setting,1024,update_day_hour_value,update_day_min_value,update_day_sec_value);
        set_antivirus_setting_item("timing-update-day",day_update_setting, set_file_path);
	
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(month_update_button)))
	{
		set_antivirus_setting_item("timing-update-select","1",set_file_path);
		update_time_set = 1;
	        printf("1111111111111 in sert\n");
		system("sed -i '/upgrade/d'  /var/spool/cron/root 2>&1 ");
                snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s %s * * sh %sstart.sh %s \" > /tmp/setupdatecron.txt 2>&1 ",update_month_min_value,update_month_hour_value,update_month_day_value,ANTIVIRUS_BIN_UPGRADE_DIR,g_current_user);
                system(crontabCmd);
	}
	
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(week_update_button)))
	{
        	update_time_set = 2;
                set_antivirus_setting_item("timing-update-select","2",set_file_path);

                if(week_num != -1)
                {
                	system("sed -i '/upgrade/d'  /var/spool/cron/root 2>&1 ");
			 printf("222222222222 in sert\n");

                        snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s * * %d sh %sstart.sh %s\" > /tmp/setupdatecron.txt 2>&1 ",update_week_min_value,update_week_hour_value,week_num+1,ANTIVIRUS_BIN_UPGRADE_DIR,g_current_user);
                        system(crontabCmd);
		}

	}
	
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(day_update_button)))
	{
		update_time_set = 3;
		printf("33333333333 in sert\n");

		set_antivirus_setting_item("timing-update-select","3",set_file_path);
		system("sed -i '/upgrade/d'  /var/spool/cron/root 2>&1 ");
                snprintf(crontabCmd,sizeof(crontabCmd),"echo \"%s %s * * * sh %sstart.sh %s\" > /tmp/setupdatecron.txt 2>&1 ",update_day_min_value,update_day_hour_value,ANTIVIRUS_BIN_UPGRADE_DIR,g_current_user);
                system(crontabCmd);
		
	}
	//memset(cmd,0,sizeof(cmd));
	//snprintf(cmd,sizeof(cmd),"sh %sstartcrontab",ANTIVIRUS_BIN_TIMEANTIVIRUS_DIR);
        //system(cmd);
	system(crontabCmd);
        memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"systemctl restart antivirusCron.service");
	system(cmd);
	
	system("rm -rf /tmp/setupdatecron.txt");

	show_warn_info(_("save sucess"),0);
	
}
void remove_devide_file_list(GtkWidget *widget,gpointer data)
{
	int i = 0;
        GtkListStore *store;
        GtkTreeModel *model;
        GtkTreeIter iter;
        gboolean value;
	char *file_name;
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(devide_file_list_list)));
        model = gtk_tree_view_get_model (GTK_TREE_VIEW(devide_file_list_list));
        if(gtk_tree_model_get_iter_first(model,&iter))
	{
                not_tog_warning(devide_file_list_list,DEVIDE_FILE_LIST_TOG_COL,FALSE);
        	do
        	{
                	i++;
                	gtk_tree_model_get(model,&iter,DEVIDE_FILE_LIST_TOG_COL,&value,-1);
			gtk_tree_model_get(model,&iter,DEVIDE_FILE_LIST_NAME_COL,&file_name,-1);
                	if(value)
			{
				char result[1024];
				strncpy(result,file_name,1024);
				char find_file_cmd[500];
				snprintf(find_file_cmd,sizeof(find_file_cmd),"grep -rn '%s' %s |cut -d ',' -f1|cut -d ':' -f2",result,ISOLATION_FILE_PATH);
				FILE *pBufSize;
				char resultBuffer[256];
				if((pBufSize = popen(find_file_cmd,"r")) == NULL)
            			{
                	
            			}
            			fgets(resultBuffer,256,pBufSize);
            			fclose(pBufSize);
				char remove_file_cmd[100];
				snprintf(remove_file_cmd,sizeof(remove_file_cmd),"rm -f %s",resultBuffer);
				system(remove_file_cmd);
                        	char cos[200];
                        	snprintf(cos,sizeof(cos),"sed -i '%dd' %s 2>&1",i,ISOLATION_FILE_PATH) ;
                        	system(cos);
                        	gtk_widget_destroy(devide_file_box);
                        	look_devide_file_settings(data);
				i--;
                	}
        	}
        	while (gtk_tree_model_iter_next(model,&iter));
	}
}
void get_devide_file_used_space(char *used_space)
{
	FILE *pBufSize;
        char cmd[100] ;
	snprintf(cmd,sizeof(cmd),"du -h %s|cut -d '/' -f1",ANTIVIRUS_DEFAULT_ISOLATION_DIR);
        if((pBufSize = popen(cmd,"r")) == NULL)
        {
        	used_space = "0.0K";

        }
        fgets(used_space,1024,pBufSize);
        fclose(pBufSize);
}
int get_select_items_num(int is_new_path)
{
      	int num = 0;
        GtkListStore *store;
        GtkTreeModel *model;
        GtkTreeIter iter;
        gboolean value;
        char *file_org_path;
        char cmd[256];
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(devide_file_list_list)));
        model = gtk_tree_view_get_model (GTK_TREE_VIEW(devide_file_list_list));
        if(gtk_tree_model_get_iter_first(model,&iter))
	{
        	do
        	{
                	gtk_tree_model_get(model,&iter,DEVIDE_FILE_LIST_TOG_COL,&value,-1);
                	gtk_tree_model_get(model,&iter,DEVIDE_FILE_LIST_NAME_COL,&file_org_path,-1);
                	if(value)
			{
                        	strncpy(devide_file,file_org_path,256);

                		if(is_new_path==0)
                		{
                        		sprintf(cmd,"bash %srecovery.sh '%s'",ANTIVIRUS_BIN_AUTO_HANDLE_DIR,devide_file);
                        		system(cmd);
                		}
                		if(is_new_path==1)
                		{
                        		sprintf(cmd,"bash %srecovery.sh '%s' '%s'",ANTIVIRUS_BIN_AUTO_HANDLE_DIR,devide_file,gtk_entry_get_text((GtkEntry *)path_entry));
                        		system(cmd);
                		}
                		++num;
                	}
        	}
        	while (gtk_tree_model_iter_next(model,&iter));
	}
        return num;
}
int write_white_list(char *logFileName ,char *white_file)
{
    	FILE *pLogFile;
    	char *pLogBuf;
    	if((pLogFile = fopen(logFileName,"ab")) == NULL)
    	{
        	return -1;
    	}
    	pLogBuf = (char *) malloc(strlen(white_file) +100);
    	sprintf(pLogBuf,"%s,\n",white_file);

    	fwrite(pLogBuf,strlen(pLogBuf),1,pLogFile);

    	free(pLogBuf);
    	fclose(pLogFile);
}
void common_recovery_default(GtkWidget *widget,gpointer data)
{
	char cmd[1024];
	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);

        set_antivirus_setting_item("boot-start","0",set_file_path);
        gtk_toggle_button_set_active(boot_start_button,FALSE);
        set_antivirus_setting_item("auto-check-mobile-dev","0", set_file_path );
        gtk_toggle_button_set_active(aotu_usb_button,FALSE);
        set_antivirus_setting_item("auto-check-download","0",set_file_path );
        gtk_toggle_button_set_active(download_button,FALSE);
	set_antivirus_setting_item("check-real-time-protect","0", set_file_path );
        gtk_toggle_button_set_active(real_time_prtect_button,FALSE);
	snprintf(cmd,sizeof(cmd),"cp %santivirus-un-use.conf /etc/antivirus/antivirus.conf",ANTIVIRUS_CONF_DIR);
	system(cmd);
        clamav_daemon_operator();
}
void anti_scan_recovery_default(GtkWidget *widget,gpointer data)
{
		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);


        set_antivirus_setting_item("anti-deal-style","1",set_file_path);
        gtk_toggle_button_set_active(intelligence_deal_button,FALSE);
        gtk_toggle_button_set_active(manual_deal_button,TRUE);
        set_antivirus_setting_item("file-into-deivide","0",set_file_path);
        gtk_toggle_button_set_active(isolation_region_button,FALSE);
        set_antivirus_setting_item("start-timing-anti","0",set_file_path);
        gtk_toggle_button_set_active(timing_anti_button,FALSE);
        set_antivirus_setting_item("timing-scan-style","2",set_file_path);
        gtk_combo_box_set_active((GtkComboBox *)anti_scan_combo, 2);
        set_antivirus_setting_item("timing-scan-select","1",set_file_path);
        auto_timing_anti_flag==1;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(month_anti_button),FALSE);
        set_antivirus_setting_item("timing-scan-month","16,12,30",set_file_path);
        gtk_combo_box_set_active(GTK_COMBO_BOX(month_anti_day_combo),15);
        gtk_combo_box_set_active(GTK_COMBO_BOX(month_anti_hour_combo),12);
        gtk_combo_box_set_active(GTK_COMBO_BOX(month_anti_min_combo),30);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_day_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_hour_combo),GTK_SENSITIVITY_OFF);
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(month_anti_min_combo),GTK_SENSITIVITY_OFF);
        set_antivirus_setting_item("timing-scan-week","1,12,30",set_file_path);
        gtk_combo_box_set_active(GTK_COMBO_BOX(week_anit_week_combo),1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(week_anit_hour_combo),12);
        gtk_combo_box_set_active(GTK_COMBO_BOX(week_anit_min_combo),30);
        set_antivirus_setting_item("timing-scan-day","12,30,30",set_file_path);
        gtk_combo_box_set_active(GTK_COMBO_BOX(day_anti_hour_combo),12);
        gtk_combo_box_set_active(GTK_COMBO_BOX(day_anit_min_combo),30);
}
void update_recovery_default(GtkWidget *widget,gpointer data)
{
		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        set_antivirus_setting_item("update-style","1",set_file_path);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(timingbutton),TRUE);
        set_antivirus_setting_item("timing-update-select","1",set_file_path);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(month_update_button),TRUE);
        set_antivirus_setting_item("timing-update-month","16,12,30",set_file_path);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_month_day),15);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_month_hour),12);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_month_min),30);
        set_antivirus_setting_item("timing-update-week","1,12,30",set_file_path);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_week_week),1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_week_hour),12);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_week_min),30);
        set_antivirus_setting_item("timing-update-day","12,30,30",set_file_path);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_day_hour),12);
        gtk_combo_box_set_active(GTK_COMBO_BOX(Combo_Box_day_min),30);
}
void leave_addbutton(GtkWidget *widget,gpointer data)
{
	char addbutton_temp[1024];
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(addbutton_temp,sizeof(addbutton_temp),_("%ssure.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *leave_addbutton_image = gtk_image_new_from_file(addbutton_temp);

        gtk_button_set_image(widget,leave_addbutton_image);
}
void enter_add_button(GtkWidget *widget,gpointer data)
{
	char addbutton_temp[1024];
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(addbutton_temp,sizeof(addbutton_temp),_("%ssure-enter.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_addbutton_image = gtk_image_new_from_file(addbutton_temp);

        gtk_button_set_image(widget,enter_addbutton_image);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_addbutton),NULL);
}
void leave_savebutton(GtkWidget *widget,gpointer data)
{
	char savebutton_temp[1024];
 	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(savebutton_temp,sizeof(savebutton_temp),_("%ssave.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_savebutton_image = gtk_image_new_from_file(savebutton_temp);
        gtk_button_set_image(widget,enter_savebutton_image);
}
void save_set_enter(GtkWidget *widget,gpointer data)
{
	gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_savebutton),NULL);
}
void leave_recovery_default(GtkWidget *widget,gpointer data)
{
	char recoverybutton_temp[1024];
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(recoverybutton_temp,sizeof(recoverybutton_temp),_("%srecovery_default.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_addbutton_image = gtk_image_new_from_file(recoverybutton_temp);

        gtk_button_set_image(widget,enter_addbutton_image);
}
void recovery_default_enter(GtkWidget *widget,gpointer data)
{
	gtk_button_set_image(widget,NULL);
	g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_recovery_default),NULL);
}
void leave_add_regular(GtkWidget *widget,gpointer data)
{
	char addregular_temp[1024];
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(addregular_temp,sizeof(addregular_temp),_("%sadd_reguar.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_addbutton_image = gtk_image_new_from_file(addregular_temp);
        gtk_button_set_image(widget,enter_addbutton_image);
}
void enter_add_regular(GtkWidget *widget,gpointer data)
{
        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_add_regular),NULL);
}
void leave_add_file(GtkWidget *widget,gpointer data)
{
	char addfile_temp[1024];
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(addfile_temp,sizeof(addfile_temp),_("%saddfile.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_addbutton_image = gtk_image_new_from_file(addfile_temp);
        gtk_button_set_image(widget,enter_addbutton_image);
}
void enter_add_file(GtkWidget *widget,gpointer data)
{
        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_add_file),NULL);
}
void leave_add_path(GtkWidget *widget,gpointer data)
{
	char addpath_image_temp[1024];
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(addpath_image_temp,sizeof(addpath_image_temp),_("%sadddire.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_addbutton_image = gtk_image_new_from_file(addpath_image_temp);
        gtk_button_set_image(widget,enter_addbutton_image);
}
void enter_add_path(GtkWidget *widget,gpointer data)
{
        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_add_path),NULL);
}
void leave_remove_button(GtkWidget *widget,gpointer data)
{
	char removebutton_temp[1024];
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(removebutton_temp,sizeof(removebutton_temp),_("%sremove.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_removebutton_image = gtk_image_new_from_file(removebutton_temp);
        gtk_button_set_image(widget,enter_removebutton_image);
}
void enter_remove_button(GtkWidget *widget,gpointer data)
{
        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_remove_button),NULL);
}
void leave_recovery_button(GtkWidget *widget,gpointer data)
{
	char recoverybutton_temp[1024];
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(recoverybutton_temp,sizeof(recoverybutton_temp),_("%srestore.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_restorebutton_image = gtk_image_new_from_file(recoverybutton_temp);
        gtk_button_set_image(widget,enter_restorebutton_image);
}
void enter_recovery_button(GtkWidget *widget,gpointer data)
{
        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_recovery_button),NULL);
}

