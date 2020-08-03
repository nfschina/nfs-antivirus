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
#include <string.h>
#include <locale.h>
#include <libintl.h>
#include <stdlib.h>
#include "../scan.h"
#include "../refresh.h"
#include "../filechooser.h"
#include "../sharememory.h"
#include "../upgrade/upgrade.h"
#include "../deal/setting_handler.h"
#include "../page/setting-page.h"
#include "../page/upgrade-page.h"
#include "../writeresult.h"
#include "../function.h"
pthread_t refreshThreadId,scanThreadId,allScanThreadId,initThreadId,selfScanThreadId,specifiedScanThreadId,iconswitchThreadId,showThreadId;
int startorendflag = 0 ; //暂停或继续标志
extern struct ScanInfo scanInfo; //扫描信息
extern fileNum;
extern scanThreatNum;
extern pathsSize;
extern scanoverSize;
extern GtkWidget *last_scan_button,*hide_button,*time_layout;
extern char scan_starttime[30];
extern char allScanExpectTime[10] ;
extern int look_devide_show;
extern int last_scan_time_show;
extern int last_update_show;
extern int start_defence_show;
extern int area;
extern GtkWidget *click_start_defence_image,*click_close_defence_image,*real_time_protect_dir,*choose_protect_button,*real_time_protect_is_label,*real_time_prtect_button;
extern GtkWidget *start_image,*close_image;
extern GtkWidget *click_start_defence_button,*click_close_defence_button;
extern GtkWidget *look_devide_label_button,*look_devide_label;
extern GtkWidget *versionlayout;
extern GtkWidget *scan_progress_value_label;
extern GtkWidget *title_layout;
extern GtkWidget *log_notebook;
int show_id=0;
pthread_t clamamDaemonOperatrorId;
extern char g_current_user[256]; //当前启动杀毒软件用户
struct SDate
{
    short int year;
    short int month;
    short int day;
};
extern struct scanResults  g_scanResultInfo;

#define _(STRING) gettext(STRING)
/***计算已经有多少天没进行扫描****/
struct SDate getDay();
void leave_antibutton(GtkWidget *widget,gpointer *data);
void leave_setbutton(GtkWidget *widget,gpointer *data);

void * clamavDaemonThread()
{
    system("/etc/init.d/cdos-antivirus-daemon restart");

}
void clamav_daemon_operator()
{
    pthread_create(&clamamDaemonOperatrorId,NULL,(void *)clamavDaemonThread,NULL);
}


/*void get_last_scan_time(char *year,char *month,char *day)
{
    char buf[100];
    FILE *scantime_file;
    char scan_ptr[30];
    char cmd[100];
    snprintf(cmd,sizeof(cmd),"cat %sscan_time.txt",ANTIVIRUS_TEMP_DIR);
    if((scantime_file = popen(cmd, "r")) != NULL)
    {
        if(fgets(scan_ptr,90, scantime_file) != NULL)
                printf("上次扫描时间是%s\n",scan_ptr);
        pclose(scantime_file);
    }
    get_year_month_day(scan_ptr,buf,year,month,day);
}*/
int get_last_scan_time(char *year,char *month,char *day)
{
    char buf[100];
    FILE *scantime_file;
    char scan_ptr[30];
    char cmd[100];
	int length = 0;
    snprintf(cmd,sizeof(cmd),"cat %s%s_scan_time.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
    printf("上次扫描的时间文件所在的位置是%s",cmd);
    if((scantime_file = popen(cmd, "r")) != NULL)
    {
		printf("111111111111111");
        if(fgets(scan_ptr,90, scantime_file) != NULL){
                printf("上次扫描时间是%s\n",scan_ptr);

        pclose(scantime_file);
        get_year_month_day(scan_ptr,buf,year,month,day);
        }
        else{
                return 0;
        }
        return 1;
    }
    else{
        printf("从scan——time。txt中获取不到内容\n");
        return 0;
    }
	/*cmd_shell_result(cmd,scan_ptr,length);
	if(length != 0)
	{
		printf("aaaaaaaaaaaaaaa\n");
        get_year_month_day(scan_ptr,buf,year,month,day);	
	}*/
	printf("bbbbbbbbbbbbb\n");
	return 0;
   // get_year_month_day(scan_ptr,buf,year,month,day);

}


int get_last_upgrade_time(char *year,char *month,char *day)
{
    char buf[100];
    FILE *scantime_file;
    char scan_ptr[30];
    char cmd[100];

//    snprintf(cmd,sizeof(cmd),"tail -1 %savlUpgradeLog.txt",ANTIVIRUS_LOG_DIR);
    snprintf(cmd,sizeof(cmd),"tail -1 %savlUpgradeDate.txt",ANTIVIRUS_LOG_DIR);
    if((scantime_file = popen(cmd, "r")) != NULL)
    {
        if(fgets(scan_ptr,90, scantime_file) != NULL)
                printf("上次扫描时间是%s\n",scan_ptr);
	else
		return 0;
        pclose(scantime_file);
    }
    get_year_month_day(scan_ptr,buf,year,month,day);
    return 1;

}


void open_defence_setting()
{

		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"/usr/share/antivirus/config/%s_setting.txt",g_current_user);

        set_antivirus_setting_item("boot-start","1",set_file_path);
                //设置开机自启动
        boot_start_deal(BOOT_START_TRUE);
        set_antivirus_setting_item("auto-check-mobile-dev","1",set_file_path);
        set_antivirus_setting_item("auto-check-download","1",set_file_path);
	set_antivirus_setting_item("check-real-time-protect","1",set_file_path);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(real_time_prtect_button),TRUE);
}

void close_defence_setting()
{
		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"/usr/share/antivirus/config/%s_setting.txt",g_current_user);


        set_antivirus_setting_item("boot-start","0",set_file_path);
                //设置取消开机自启动
        boot_start_deal(BOOT_START_FALSE);
        set_antivirus_setting_item("auto-check-mobile-dev","0",set_file_path);
        set_antivirus_setting_item("auto-check-download","0",set_file_path);
	set_antivirus_setting_item("check-real-time-protect","0",set_file_path);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(real_time_prtect_button),FALSE);
}


void compute_scan_day(char *day_dif)
{
    struct SDate scanday,today;
    char buf[100],year[10],month[5],day[5];
    char now[30];
    FILE *scantime_file;
    char scan_ptr[30];
    char cmd[100];

    snprintf(cmd,sizeof(cmd),"cat %s%s_scan_time.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
    if((scantime_file = popen(cmd, "r")) != NULL)
    {
        if(fgets(scan_ptr,90, scantime_file) != NULL)
        	printf("上次扫描时间是%s\n",scan_ptr);
	pclose(scantime_file);
    }
    get_year_month_day(scan_ptr,buf,year,month,day);

    scanday.year = atoi(year);
    scanday.month = atoi(month);
    scanday.day = atoi(day);
    getsystime();
    get_year_month_day(scan_starttime,buf,year,month,day);
    today.year = atoi(year);
    today.month = atoi(month);
    today.day = atoi(day);

    int days;
    days=SDateToAbsDays(today)-SDateToAbsDays(scanday);
    sprintf(day_dif,"%d",days);
}
/****创建扫描文件线程*****/
void scan_file_thread(int scanType,char *scanPath)
{
    int ret = 0;
    if(scanType == CUSTOM_SCAN)
    {
        ret = pthread_create(&scanThreadId,NULL,(void *)selfscanThread,NULL);
    }
    else if(scanType == SPECIFIED_SCAN || scanType == FIREFOX_DOWNLOAD_SCAN  || scanType == CLICK_DIR_SCAN ||  scanType == CLICK_FILE_SCAN )
    {
        ret = pthread_create(&specifiedScanThreadId,NULL,(void *)specifiedScanThread,scanPath);
    }
    else
    {
        ret = pthread_create(&scanThreadId,NULL,(void *)accord_type_scan_file,&scanInfo.scanType);
    }
    if(ret != 0)
    {
        exit(1);
    }


}

/****创建刷新扫描界面线程***/
void refresh_scan_page_thread()
{
    int ret = 0;
    g_thread_new("refresh", refreshPageThread, &scanInfo);
}

/*******检查扫描状态***********/
int checkScanStatus(int scanType)
{
    int avlStatus = 0;
    char warnMessage[255] ;
    char upgrade_status_flag[512];
    snprintf(upgrade_status_flag,sizeof(upgrade_status_flag),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    avlStatus = get_upgrde_status(upgrade_status_flag);
    if( avlStatus !=AVL_INIT_OVER  && avlStatus!=AVL_UPGADE_END && avlStatus!= AVL_SWITCH_PAGE_END )
    {

        if(scanType != scanInfo.scanType)
        {
            //提示信息赋值
            if(scanInfo.scanType == FAST_SCAN)
            {
                strncpy( warnMessage,_("Currently being quick scanned, please make a scan in the current after scanning."),sizeof(warnMessage));
            }
            else if(scanInfo.scanType == ALL_SCAN)
            {
                strncpy( warnMessage,_("Currently being all scanned, please make a scan in the current after scanning."),sizeof(warnMessage));
            }
            else if(scanInfo.scanType == CUSTOM_SCAN)
            {
                strncpy( warnMessage,_("Currently being custom scanned, please make a scan in the current after scanning."),sizeof(warnMessage));
            }
            else if(scanInfo.scanType == SPECIFIED_SCAN)
            {
                strncpy( warnMessage,_("Currently being usb scanned, please make a scan in the current after scanning."),sizeof(warnMessage));
            }
            else if(scanInfo.scanType == FIREFOX_DOWNLOAD_SCAN )
            {
                strncpy( warnMessage,_("Currently being firefox download scanned, please make a scan in the current after scanning."),sizeof(warnMessage));
            }

            //显示提示信息
            if(scanType != SPECIFIED_SCAN)
            {
                show_warn_info(warnMessage,1);
            }
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


/***********重新初始化刷新界面****************/
void init_refresh_page_info()
{
        char semkeyName[30] = "scanStatusMutexKey";
        char shmkeyName[30] = "scanResultInfoKey";
        char scanExpectTime[10]= {0};
        struct scanResults scanNullInfo;
        char useTime[45]="00:00:00";
        char scantimelabel[30];
        char scanFileName[30];
        char expectscantimelabel[30];
        char scanObj[30];
        char finddangerlabel[30];
        //清空共享内存
        strcpy(g_scanResultInfo.filename,"");
        g_scanResultInfo.scanStatus = SCAN_CONTINUE;
        g_scanResultInfo.scanObjNum = 0;
        g_scanResultInfo.scanThreatNum = 0;
        strcpy(g_scanResultInfo.scanExpectTime,scanExpectTime );
        g_scanResultInfo.filesize = 0;
        g_scanResultInfo.pathsSize = 0;
        //write_share_memory(semkeyName , shmkeyName, &scanNullInfo);
        fileNum = 0;
        scanThreatNum = 0;
        pathsSize = 0;
        scanoverSize = 0;

        //进度条置0
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(scanInfo.progress_bar), 0.0);

        gtk_label_set_text(scan_progress_value_label,"0%");
        change_font_style((GtkLabel *)scan_progress_value_label,"<span foreground=\"#3dbd8f\" font_desc='16'>%s</span>");

        //执行时间置0
        sprintf(scantimelabel,_("scan time:%s"),useTime);
        gtk_label_set_text(scanInfo.scantimelabel,scantimelabel);
        //设置标签样式
        set_lable_style(scanInfo.scantimelabel,"<span foreground=\"#ffffff\" font_desc='12'>%s</span>");
        //预计时间置0
	 if(scanInfo.scanType != ALL_SCAN)
        {
                 //预计时间置0
                sprintf(expectscantimelabel,_("expect time:%s"),"");
                gtk_label_set_text(scanInfo.expectscantimelabel,expectscantimelabel);
                //设置标签样式
                set_lable_style(scanInfo.expectscantimelabel,"<span foreground=\"#ffffff\" font_desc='12'>%s</span>");
                gtk_label_set_text(scanInfo.allScanTipLabel,"");
        }
        else
        {
                gtk_label_set_text(scanInfo.expectscantimelabel,"");
                gtk_label_set_text(scanInfo.allScanTipLabel,_("For a while, please be patient."));
                change_font_style((GtkLabel *)scanInfo.allScanTipLabel,"<span foreground=\"#3dbd8f\" font_desc='14'>%s</span>");


        }
        //正在扫描的文件置空
	sprintf(scanFileName,_("scaning file:%s"),"");
        gtk_label_set_text(scanInfo.scaningfilenamelabel,scanFileName);
	change_font_style((GtkLabel *)scanInfo.scaningfilenamelabel,"<span foreground=\"#4c4554\" font_desc='14'>%s</span>");
        //扫描对象个数置0
        sprintf(scanObj,_("scan object:%d"),0);
        gtk_label_set_text(scanInfo.scanobjlabel,scanObj);
        //设置标签样式
        set_lable_style(scanInfo.scanobjlabel,"<span foreground=\"#ffffff\" font_desc='12'>%s</span>");
        //发现威胁置0
        sprintf(finddangerlabel,_("that threat:%d"),0);
        gtk_label_set_text(scanInfo.finddangerlabel,finddangerlabel);
        //设置标签样式
        set_lable_style(scanInfo.finddangerlabel,"<span foreground=\"#ffffff\" font_desc='12'>%s</span>");

}



/******* ********************************* 

******函数功能：
      执行扫描文件
******
        
******函数参数：
******@param scanType 扫描类型:
        FAST_SCAN 快速扫描、ALL_SCAN 全盘扫描

************************** ************/
void execution_scan(int scanType,char *scanPath)
{
    int retScanStatus = -1;
   
    if(scanType != CUSTOM_SCAN )
    {
         //检查当前是否正在扫描
        retScanStatus = checkScanStatus(scanType);
        //正在扫描，且类型不相同
        if( retScanStatus ==1 )
        {
                return;
        }
        else if ( retScanStatus ==2 )
        {
                page_switch_to_scan();
                return ;
        }
    }

    startorendflag=0;
    //重新赋值扫描类型
    scanInfo.scanType = scanType;
    time(&scanInfo.startScanTime);
    init_refresh_page_info();
    //删除上次扫描结果
    char scan_cmd[1024];
    snprintf(scan_cmd,sizeof(scan_cmd),"rm -rf %s%s_scan_results.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
    system(scan_cmd);
    //将扫描状态写入文件
    memset(scan_cmd,0,sizeof(scan_cmd));
    snprintf(scan_cmd,sizeof(scan_cmd),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    write_antivirus_status(scan_cmd,"3");
    //将界面从主界面切换到扫描过程界面 
    page_switch_to_scan();
    //创建刷新扫描界面线程
    refresh_scan_page_thread();
    //获取扫描开始时间
    getsystime();
    sprintf(scan_cmd,"echo %s >%s%s_scan_time.txt",scan_starttime,ANTIVIRUS_TEMP_DIR,g_current_user);
    system(scan_cmd);
    //全盘扫描获取预计时间
    //创建扫描文件线程
    scan_file_thread(scanType,scanPath);

}
void show_last_scan_time(GtkWidget *widget,gpointer *data)
{
        gtk_widget_hide(GTK_WIDGET(data));
        gtk_widget_show(time_layout);
}
void hide_last_scan_time(GtkWidget *widget,gpointer *data)
{
	if(last_scan_time_show==0)
	{
		gtk_widget_show(GTK_WIDGET(data));
		last_scan_time_show = 1;
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(data));
		last_scan_time_show = 0;
	}

}
void show_or_hide_look_devide_button(GtkWidget *widget,gpointer *data)
{
	if(look_devide_show==0)
	{
		gtk_widget_show(data);
		look_devide_show = 1;
	}
	else
	{
		gtk_widget_hide(data);
		look_devide_show = 0;
	}
}
void show_or_hide_last_update(GtkWidget *widget,gpointer *data)
{
	if(last_update_show==0)
	{
                gtk_widget_show(data);
                last_update_show = 1;
        }
        else
	{
                gtk_widget_hide(data);
                last_update_show = 0;
        }
}

void showThread(GtkWidget *widget)
{
       	while (show_id<5)
        {
		if (get_defence_status()==1)
                {
                        gtk_widget_hide(click_start_defence_button);
                        gtk_widget_show(start_image);
                }
                else
                {

                        gtk_widget_hide(click_close_defence_button);
                        gtk_widget_show(close_image);
                }
                sleep(1);
                show_id++;
        }
        if (show_id=5)
        {
                gtk_widget_hide(widget);
                show_id=0;
		if(get_defence_status()==1)
		{
                        gtk_widget_hide(start_image);
                        gtk_widget_show(click_close_defence_button);
                }
                else
		{
                        gtk_widget_hide(close_image);
                        gtk_widget_show(click_start_defence_button);
                }
        }
}


void start_defence(GtkWidget *widget,gpointer *data)
{
	char defense_cmd[1024];
	char defense_image[1024];
	snprintf(defense_cmd,sizeof(defense_cmd),"cp %santivirus-use.conf /etc/antivirus/antivirus.conf",ANTIVIRUS_CONF_DIR);
	snprintf(defense_image,sizeof(defense_image),_("%sopen_defence.gif"),ANTIVIRUS_IMAGE_DIR);
	if((check_start_real_protect()==0)||(check_start_real_protect()==2))
	{
		system(defense_cmd);
                clamav_daemon_operator();
  	}
	char defence_log[1024];
//        char *defence_log_type=_("boot-start");
	char defence_log_type[1024];
        strcpy(defence_log_type,_("boot-start"));
        char *defenc_log_result=_("open");
        getsystime();
        snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
        write_defence_log(defence_log);
        delete_log_30daysago("defence_log");
       	open_defence_setting();
	gtk_image_set_from_file(data,defense_image);
	gtk_widget_hide(click_start_defence_button);
	gtk_widget_show(click_close_defence_button);
        gtk_widget_show(data);
}
void close_defence(GtkWidget *widget,gpointer *data)
{
	char defense_cmd[1024];
        char defense_image[1024];
        snprintf(defense_cmd,sizeof(defense_cmd),"cp %santivirus-un-use.conf /etc/antivirus/antivirus.conf",ANTIVIRUS_CONF_DIR);
        snprintf(defense_image,sizeof(defense_image),_("%sclose_defence.gif"),ANTIVIRUS_IMAGE_DIR);
	if((check_start_real_protect()==0)||(check_start_real_protect()==1))
	{
		system(defense_cmd);
             	clamav_daemon_operator();
        }
	char defence_log[1024];
//        char *defence_log_type=_("boot-start");
	char defence_log_type[1024];
        strcpy(defence_log_type,_("boot-start"));
        char *defenc_log_result=_("close");
        getsystime();
        snprintf(defence_log,sizeof(defence_log),"%s,%s,%s,",scan_starttime,defence_log_type,defenc_log_result);
        write_defence_log(defence_log);
        delete_log_30daysago("defence_log");
	close_defence_setting();
	gtk_image_set_from_file(data,defense_image);
	gtk_widget_show(data);
	gtk_widget_hide(click_close_defence_button);
	gtk_widget_show(click_start_defence_button);
	
}
void look_devide(GtkWidget *widget,gpointer *data)
{
	setwindow(NULL,NULL);
	gtk_notebook_set_current_page(notebook,4);
}

void look_update_time(GtkWidget *widget,gpointer *data)
{
//	setwindow(NULL,NULL);
	logwindow(NULL,NULL);
	gtk_notebook_set_current_page(log_notebook,2);
}

void check_update(GtkWidget *widget,gpointer *data)
{
	char pre_runupdate_cmd[1024];
	memset(pre_runupdate_cmd,0,sizeof(pre_runupdate_cmd));
	snprintf(pre_runupdate_cmd,sizeof(pre_runupdate_cmd),"if [ -f %s%s_avlUpgradeStartStatus.txt ];then rm -rf %s%s_avlUpgradeStartStatus.txt;elif [ -f %s%s_avlUpgradeStatus.txt ];then rm -rf %s%s_avlUpgradeStatus.txt ; fi",ANTIVIRUS_TEMP_DIR,g_current_user,ANTIVIRUS_TEMP_DIR,g_current_user,ANTIVIRUS_TEMP_DIR,g_current_user,ANTIVIRUS_TEMP_DIR,g_current_user);
	system(pre_runupdate_cmd);
        updatewindow(NULL,NULL);
}

void feedback_func(GtkWidget *widget,gpointer *data)
{
	char feedback_cmd[1024];
	snprintf(feedback_cmd,sizeof(feedback_cmd),"su -c '%santifeedback' %s",ANTIVIRUS_BIN_DIR,g_current_user);
	system(feedback_cmd);
}
void enter_antibutton(GtkWidget *widget,gpointer *data)
{
        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_antibutton),NULL);
}
void leave_antibutton(GtkWidget *widget,gpointer *data)
{
	char antibutton_image_def[1024];
        if(area==1)
	{
		snprintf(antibutton_image_def,sizeof(antibutton_image_def),_("%santi_enter.png"),ANTIVIRUS_IMAGE_DIR);
        	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        	GtkWidget *leave_antibutton_image = gtk_image_new_from_file(antibutton_image_def);

        	gtk_button_set_image(widget,leave_antibutton_image);
        	return;
        }
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	memset(antibutton_image_def,0,sizeof(antibutton_image_def));
	snprintf(antibutton_image_def,sizeof(antibutton_image_def),_("%santi.png"),ANTIVIRUS_IMAGE_DIR);	
        GtkWidget *leave_antibutton_image = gtk_image_new_from_file(antibutton_image_def);
        gtk_button_set_image(widget,leave_antibutton_image);
}
void enter_setbutton(GtkWidget *widget,gpointer *data)
{
        gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_setbutton),NULL);
}
void leave_setbutton(GtkWidget *widget,gpointer *data)
{
	char setbutton_image_temp[1024];
        if(area==2)
	{
                gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
		snprintf(setbutton_image_temp,sizeof(setbutton_image_temp),_("%sset_enter.png"),ANTIVIRUS_IMAGE_DIR);
        	GtkWidget *leave_antibutton_image = gtk_image_new_from_file(setbutton_image_temp);

        	gtk_button_set_image(widget,leave_antibutton_image);
                return ;
        }
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	memset(setbutton_image_temp,0,sizeof(setbutton_image_temp));
	snprintf(setbutton_image_temp,sizeof(setbutton_image_temp),_("%ssetting.png"),ANTIVIRUS_IMAGE_DIR);	
        GtkWidget *leave_setbutton_image = gtk_image_new_from_file(setbutton_image_temp);

        gtk_button_set_image(widget,leave_setbutton_image);
}
void leave_min_button(GtkWidget *widget,gpointer *data)
{
	char minbutton_image_temp[1024];
	snprintf(minbutton_image_temp,sizeof(minbutton_image_temp),_("%smini.png"),ANTIVIRUS_IMAGE_DIR);
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	GtkWidget *leave_antibutton_image = gtk_image_new_from_file(minbutton_image_temp);
        gtk_button_set_image(widget,leave_antibutton_image);
        gtk_layout_move(title_layout,widget,745,0.5);
}
void enter_min_button(GtkWidget *widget,gpointer *data)
{
	gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_min_button),NULL);
        gtk_layout_move(title_layout,widget,745,4);

}
void leave_close_button(GtkWidget *widget,gpointer *data)
{
	char closebutton_temp[1024];
	snprintf(closebutton_temp,sizeof(closebutton_temp),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        GtkWidget *leave_antibutton_image = gtk_image_new_from_file(closebutton_temp);

        gtk_button_set_image(widget,leave_antibutton_image);
        gtk_layout_move(title_layout,widget,770,0.5);

}
void enter_close_button(GtkWidget *widget,gpointer *data)
{
	gtk_button_set_image(widget,NULL);
        gtk_layout_move(title_layout,widget,770,4);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_close_button),NULL);
}

void leave_quickbutton(GtkWidget *widget,gpointer *data)
{
        int avlStatus = 0;
	char avlstatus_temp[512];
	char leave_quickbutton_temp[1024];
	snprintf(avlstatus_temp,sizeof(avlstatus_temp),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
        avlStatus = get_upgrde_status(avlstatus_temp);
        if(avlStatus==3)
	{
                if(scanInfo.scanType == FAST_SCAN)
		{
                        return;
                }
        }

        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(leave_quickbutton_temp,sizeof(leave_quickbutton_temp),_("%squicksacn.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_setbutton_image = gtk_image_new_from_file(leave_quickbutton_temp);

        gtk_button_set_image(widget,enter_setbutton_image);
}
void enter_quick_button(GtkWidget *widget,gpointer *data)
{
	char enter_quickbutton_temp[1024];
	snprintf(enter_quickbutton_temp,sizeof(enter_quickbutton_temp),_("%squickscan_down.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        GtkWidget *enter_setbutton_image = gtk_image_new_from_file(enter_quickbutton_temp);

        gtk_button_set_image(widget,enter_setbutton_image);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_quickbutton),NULL);
}
void leave_allbutton(GtkWidget *widget,gpointer *data)
{
        int avlStatus = 0;
	char avlstatus_temp[512];
	char leave_allbutton_temp[1024];
	snprintf(avlstatus_temp,sizeof(avlstatus_temp),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
        avlStatus = get_upgrde_status(avlstatus_temp);
        if(avlStatus==3)
	{
                if(scanInfo.scanType == ALL_SCAN)
		{
                        return;
                }
        }

        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(leave_allbutton_temp,sizeof(leave_allbutton_temp),_("%sallscan.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_setbutton_image = gtk_image_new_from_file(leave_allbutton_temp);

        gtk_button_set_image(widget,enter_setbutton_image);
}
void enter_allscan(GtkWidget *widget,gpointer *data)
{
	char enter_allscan_temp[1024];
	snprintf(enter_allscan_temp,sizeof(enter_allscan_temp),_("%sallscan_down.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        GtkWidget *enter_setbutton_image = gtk_image_new_from_file(enter_allscan_temp);

        gtk_button_set_image(widget,enter_setbutton_image);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_allbutton),NULL);
}
void leave_myselfbutton(GtkWidget *widget,gpointer *data)
{
        int avlStatus = 0;
	char avlstatus_temp[512];
	char leave_myselfbutton_temp[1024];
	snprintf(avlstatus_temp,sizeof(avlstatus_temp),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
        avlStatus = get_upgrde_status(avlstatus_temp);
        if(avlStatus==3)
	{
                if(scanInfo.scanType == CUSTOM_SCAN)
		{
                        return;
                }
        }

        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	snprintf(leave_myselfbutton_temp,sizeof(leave_myselfbutton_temp),_("%smyselfscan.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_setbutton_image = gtk_image_new_from_file(leave_myselfbutton_temp);

        gtk_button_set_image(widget,enter_setbutton_image);
}
void enter_myselfscan(GtkWidget *widget,gpointer *data)
{
	char enter_myselfbutton_temp[1024];
	snprintf(enter_myselfbutton_temp,sizeof(enter_myselfbutton_temp),_("%smyselfscan_down.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        GtkWidget *enter_setbutton_image = gtk_image_new_from_file(enter_myselfbutton_temp);

        gtk_button_set_image(widget,enter_setbutton_image);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_myselfbutton),NULL);
}
void leave_start_defence(GtkWidget *widget,gpointer *data)
{
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	gtk_widget_show(data);
}
void enter_start_defence(GtkWidget *widget,gpointer *data)
{
	gtk_widget_hide(data);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_start_defence),data);
}
void leave_close_defence(GtkWidget *widget,gpointer *data)
{
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
	gtk_widget_show(data);
}
void enter_close_defence(GtkWidget *widget,gpointer *data)
{
	gtk_widget_hide(data);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_close_defence),data);
}
void leave_devide_label_button(GtkWidget *widget,gpointer *data)
{
	gtk_widget_show(look_devide_label);
}
void enter_look_devide_button(GtkWidget *widget,gpointer *data)
{
	gtk_widget_hide(look_devide_label);
	g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_devide_label_button),NULL);
}
void leave_checkupdate(GtkWidget *widget,gpointer *data)
{
	gtk_layout_move(versionlayout,widget,142,-4);
	gtk_widget_show(data);
}
void enter_checkupdate(GtkWidget *widget,gpointer *data)
{
	gtk_layout_move(versionlayout,widget,142,0);	
	gtk_widget_hide(data);
	g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_checkupdate),data);
}
void leave_checkupdate_white_bg(GtkWidget *widget,gpointer *data)
{
	gtk_layout_move(versionlayout,widget,142,0);
}
void enter_checkupdate_white_bg(GtkWidget *widget,gpointer *data)
{
	gtk_layout_move(versionlayout,widget,142,0);
	g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_checkupdate_white_bg),data);
}
void leave_feedback(GtkWidget *widget,gpointer *data)
{
	gtk_layout_move(versionlayout,widget,20,-4);
	gtk_widget_show(data);
}
void enter_feedback(GtkWidget *widget,gpointer *data)
{
	gtk_layout_move(versionlayout,widget,20,0);
	gtk_widget_hide(data);
	g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_feedback),data);
}
void leave_feedback_white_gb(GtkWidget *widget,gpointer *data)
{
	gtk_layout_move(versionlayout,widget,20,0);
}
void enter_feedback_white_bg(GtkWidget *widget,gpointer *data)
{
	gtk_layout_move(versionlayout,widget,20,0);
	g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_feedback_white_gb),data);
}
