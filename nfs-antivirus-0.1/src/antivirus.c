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
#include "scan.h"
#include "refresh.h"
#include <sys/stat.h>
#include "writelog.h"
#include "detect_usb/detect_usb.h"
#include "function.h"
#include "page/setting-page.h"
#include "page/home-page.h"
#include "deal/log_handle.h"
#include "function.h"
#define _(STRING) gettext(STRING)
extern struct cl_engine *engine;
extern GtkWindow *window ;
int avlInit = 0;
struct ScanInfo scanInfo;
extern GtkLabel * attentionlabel;
extern GtkWidget *title_layout, *versionimage;
extern GtkWidget *scan_process_title_layout,*scan_process_title_image;
GtkWidget *layout_main;
extern GtkWidget *real_protect_result_table;
extern GtkStatusIcon *trayIcon ;
extern int area;
extern GtkWidget *detail_result_table;
GtkWidget *box,*while_list_content_box;
GtkWidget *tip_dialog;
pthread_t allScanExpectTimeThreadId ;
extern GtkWidget *notebook;
GtkWidget *timingtext,*periodtext,*updateporgress,*updatelabel,*updatelabel1,*savetimebutton,*higtbutton,*comprehensivebutton;
pthread_t avlInitThreadId, waitInitThreadId;
gdouble value;
extern GtkWidget *feedback_label_button_white_bg,*feedback_label_button,*check_update_button,*check_update_button_white_bg;
/** table1为扫描按钮，table6为扫描过程界面*/
extern GtkWidget *table1,*table2,*table3,*table4,*table6,*table7,*auto_scan_usb_opt;
/**杀毒软件主界面菜单栏***/
extern GtkWidget *filew;
extern int import_type;
extern char *improt_filename;
GtkWidget *back_to_mainmenu_button;
void switch_to_mainmenu(); /***返回主界面***/
void page_switch_to_scan();/*****显示扫描过程界面*********/
void check_choose_detect_usb(GtkWidget *check_button);
int AutoStartFirstShowFlag  = 0;
GtkTreeModel *createlogModel(char *filepath);
char g_current_user[256]; //当前启动杀毒软件用户

void  destroy_tipdialog(GtkWidget *widget ,gpointer *data)
{
       	if((strcmp(data,_("sure-to-quit")))==0)
	{
        	system("rm -f /tmp/.user");
//		char write_quit_anti_flag_file[512];
//		snprintf(write_quit_anti_flag_file,sizeof(write_quit_anti_flag_file),"echo 0 >%srun_anti_flag_file",ANTIVIRUS_CONF_DIR);
//		printf("命令为%s\n",write_quit_anti_flag_file);
//		system(write_quit_anti_flag_file);

		gtk_widget_destroy(tip_dialog);
	}
	else if((strcmp(data,_("Are you sure to stop scaning?")))==0)
	{
		 stop_scaning_no();
		gtk_widget_destroy(tip_dialog);
	}
	else if((strcmp(data,_("file exist,sure to cover")))==0){
                gtk_widget_destroy(tip_dialog);
                gtk_widget_destroy(filew);
        }
	else{	
		printf("保存窗口关闭退出情况\n");
		gtk_widget_destroy(tip_dialog);
	}

}

//退出引擎初始化函数
void quit_init_engine()
{
        g_source_remove(avlInitThreadId);
        pthread_cancel(avlInitThreadId);
        waitScanThreadCacelFinish(avlInitThreadId);
}

void  save_tipdialog(GtkWidget *widget ,gpointer *data)
{
	gtk_widget_destroy(tip_dialog);
	if((strcmp(data,_("sure-to-quit")))==0){
		char write_quit_anti_flag_file[512];
                snprintf(write_quit_anti_flag_file,sizeof(write_quit_anti_flag_file),"echo 0 >%srun_anti_flag_file",ANTIVIRUS_CONF_DIR);
                printf("命令为%s\n",write_quit_anti_flag_file);
                system(write_quit_anti_flag_file);
	
            char upgrade_dir[512];
           snprintf(upgrade_dir,sizeof(upgrade_dir),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);

		int scan_status = get_upgrde_status(upgrade_dir);
		if(scan_status==3){
			int scan_type = get_scanType();
			if(scan_type==1){
				quitallscan(NULL,NULL);
     			}
			if(scan_type==2){
				quitselfscan(NULL,NULL);
			}
			if(scan_type==3){
				quitselfscan(NULL,NULL);
			}
			if (scanInfo.scanType == SPECIFIED_SCAN || scanInfo.scanType == FIREFOX_DOWNLOAD_SCAN)
                	{
                        quitSpecifiedScan(NULL,NULL);
                	}

		}
		
                if(scan_status == 1)
                {
                     quit_init_engine();
                }



		gtk_widget_destroy(window);
}
	if((strcmp(data,_("this deal will deal thread from one file!")))==0)
	{
		remove_item_real();
	}
	if((strcmp(data,_("this deal will add thread from one file!")))==0)
	{
		add_white_list_real();		
	}
	if((strcmp(data,_("Are you sure to stop scaning?")))==0)
        {
                 stop_scaning_yes();
        }
	if((strcmp(data,_("file exist,sure to cover")))==0){
                char rm_cmd[1024];
                snprintf(rm_cmd,sizeof(rm_cmd),"rm -f %s",improt_filename);
                system(rm_cmd);
                import_log_func(import_type,improt_filename);
                //cancle_scan_log_improt=1;
                gtk_widget_destroy(tip_dialog);
                gtk_widget_destroy(filew);
        }

}

void waitInitThread(GtkLabel *attentionlabel){
    void* status;
    char upgrade_dir[512];
           snprintf(upgrade_dir,sizeof(upgrade_dir),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    gchar all_totleattentionlabel[100];
    memset(all_totleattentionlabel, 0, sizeof(all_totleattentionlabel));
    pthread_join(avlInitThreadId, &status);
    if(attentionlabel){
       
	    

    	char anti_ver[1024] = "0.102.3" ;
        //temp modify sff 
        //cmd_shell_result("dpkg -s cdos-antivirus | grep Version | cut -d ':' -f2",anti_ver,1024);
        //anti_ver[strlen(anti_ver)-1]=0;
	char version_info[1024];
	snprintf(version_info,sizeof(version_info),_("Current version:%s"),anti_ver);

	//char engin_ver[1024];
	//cmd_shell_result("dpkg -s cdos-virus | grep Description|cut -d ':' -f2 |sed s/[[:space:]]//g",engin_ver,1024);
	//engin_ver[strlen(engin_ver)-1]=0;
	//char engin_title[1024];
	//snprintf(engin_title,sizeof(engin_title),_("Engine version:%s"),engin_ver);
	gtk_label_set_text(attentionlabel,version_info);
        change_font_style((GtkLabel *)attentionlabel,"<span foreground=\"#74787c\" font_desc='10'>%s</span>");
    }
    avlInit = 1;
    write_antivirus_status(upgrade_dir,"2");
    return ;
}


int checkEngineStatus(GtkWidget *widget,gpointer data){
    int avlStatus = 0;
    char upgrade_dir[512];
           snprintf(upgrade_dir,sizeof(upgrade_dir),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    avlStatus = get_upgrde_status(upgrade_dir);
    if(avlInit == 0 || avlStatus == 6 ){
        initWarn(widget, data);

        if(avlInit == 0)
        {
            return 1;
        }
        if(avlStatus == 6)
        {
            return 2;
        }

    }else{
        return 0;
    }
}
void leave_close_tipdialog_button(GtkWidget *widget,gpointer data){
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);
        char image_close[512];
           snprintf(image_close,sizeof(image_close),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_setbutton_image = gtk_image_new_from_file(image_close);

        gtk_button_set_image(widget,enter_setbutton_image);
}
void enter_close_tipdialog_button(GtkWidget *widget,gpointer data){
	gtk_button_set_image(widget,NULL);
        g_signal_connect(G_OBJECT(widget),"leave",G_CALLBACK(leave_close_tipdialog_button),NULL);
}
/*****显示提示信息*********/
void  show_warn_info(char *warnMessage,int dailog_type)
{
   GtkWidget *surebutton,*canclebutton,*label,*tipdialog_layout,*tipdialog_bg_image,*close_tipdialog_button,*close_tipdialog_image,*tip_image,*tip_label,*sure_button,*sure_button_image,*cancel_button,*cancel_button_image;
    char image_tipdialog_bg_image[512];
    snprintf(image_tipdialog_bg_image,sizeof(image_tipdialog_bg_image),_("%stipdialog_bg_image.png"),ANTIVIRUS_IMAGE_DIR);
    char css_tipdialog[512];
    snprintf(css_tipdialog,sizeof(css_tipdialog),"%stipdialog-chinese.css",ANTIVIRUS_CSS_DIR);
   tip_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_transient_for(tip_dialog,window);
   gtk_window_set_modal(tip_dialog,TRUE);
   gtk_window_set_decorated(GTK_WINDOW(tip_dialog), FALSE);
   gtk_window_set_position(GTK_WINDOW(tip_dialog),GTK_WIN_POS_CENTER);
   gtk_widget_set_events(GTK_WINDOW(tip_dialog),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

    g_signal_connect(GTK_WINDOW(tip_dialog), "button_press_event", G_CALLBACK (button_press_event), tip_dialog);       // 加入事件回调
//    g_signal_connect(GTK_WINDOW(tip_dialog), "motion_notify_event",G_CALLBACK( motion_notify_event), tip_dialog);
    g_signal_connect(GTK_WINDOW(tip_dialog), "button_release_event", G_CALLBACK(button_release_event), tip_dialog);
    tipdialog_layout = gtk_layout_new(NULL, NULL);
   if((strcmp(warnMessage,_("save sucess")))==0||(strcmp(warnMessage,_("Engine is initing...\nWait for a moment")))==0){	
	printf("保存成功大小设置\n");
        gtk_widget_set_size_request(tipdialog_layout, 308, 200);
	if(strcmp(warnMessage,_("Engine is initing...\nWait for a moment"))==0){
		gtk_window_set_title(tip_dialog,_("warning"));	
	}
	else{
		gtk_window_set_title(tip_dialog,_("save sucess"));
	}
	printf("跳出这个窗口大小设置\n");
	}
   else if((strcmp(warnMessage,_("sure-to-quit")))==0||(strcmp(warnMessage,_("Are you sure to stop scaning?")))==0||(strcmp(warnMessage,_("file exist,sure to cover")))==0)
	{
	printf("退出大小设置\n");	
        gtk_widget_set_size_request(tipdialog_layout, 308, 200);
	gtk_window_set_title(tip_dialog,_("confirm quit"));
	}
    else if(strcmp(warnMessage,_("Any item not selected,please select item to deal again!"))==0){
	printf("没有选择任何数据项\n");
		gtk_widget_set_size_request(tipdialog_layout, 378, 246);
		gtk_window_set_title(tip_dialog,_("warning"));
	}
    else if (strcmp(warnMessage,_("this deal will deal thread from one file!"))==0)
    {
	gtk_widget_set_size_request(tipdialog_layout, 378, 200);
    }
   else{
	printf("其他大小设置\n");
        gtk_widget_set_size_request(tipdialog_layout, 378, 246);
	}
    tipdialog_bg_image = gtk_image_new_from_file(image_tipdialog_bg_image);
    gtk_widget_show(tipdialog_bg_image);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),tipdialog_bg_image, 0, 0);

    GFile *file=g_file_new_for_path(css_tipdialog);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);
    printf("保存关闭窗口代码走到这里啦\n");
    close_tipdialog_button = gtk_button_new();
    gtk_button_set_relief(close_tipdialog_button,GTK_RELIEF_NONE);
    gtk_widget_show(close_tipdialog_button);
//    g_signal_connect(G_OBJECT(close_tipdialog_button),"enter",G_CALLBACK(enter_close_tipdialog_button),NULL);
    printf("开始设置关闭按钮事件\n");
    g_signal_connect(G_OBJECT(close_tipdialog_button),"clicked",G_CALLBACK(destroy_tipdialog),warnMessage);
    memset(image_tipdialog_bg_image,0,sizeof(image_tipdialog_bg_image));
    snprintf(image_tipdialog_bg_image,sizeof(image_tipdialog_bg_image),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
    close_tipdialog_image = gtk_image_new_from_file(image_tipdialog_bg_image);
    gtk_widget_show(close_tipdialog_image);
  //  gtk_button_set_image(close_tipdialog_button,close_tipdialog_image);
    gtk_widget_set_size_request(close_tipdialog_button,24,24);
    gtk_widget_set_name(close_tipdialog_button,"tipdialog-close");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
    if((strcmp(warnMessage,_("save sucess")))==0||(strcmp(warnMessage,_("Engine is initing...\nWait for a moment")))==0)
        {        
		printf("设置一个按钮的代码部分\n");
		gtk_layout_put(GTK_LAYOUT(tipdialog_layout),close_tipdialog_button, 280, 3);
		printf("开始设置关闭按钮的代码部分\n");
		g_signal_connect(G_OBJECT(close_tipdialog_button),"clicked",G_CALLBACK(destroy_tipdialog),warnMessage);
	}
    else if((strcmp(warnMessage,_("sure-to-quit")))==0||(strcmp(warnMessage,_("Are you sure to stop scaning?")))==0||(strcmp(warnMessage,_("file exist,sure to cover")))==0)
                gtk_layout_put(GTK_LAYOUT(tipdialog_layout),close_tipdialog_button, 280, 3);
    else
        gtk_layout_put(GTK_LAYOUT(tipdialog_layout),close_tipdialog_button, 355, 3);
//    g_signal_connect(G_OBJECT(close_tipdialog_button),"enter",G_CALLBACK(enter_close_tipdialog_button),NULL);
//    g_signal_connect(G_OBJECT(close_tipdialog_button),"clicked",G_CALLBACK(destroy_tipdialog),warnMessage);
    memset(image_tipdialog_bg_image,0,sizeof(image_tipdialog_bg_image));
    snprintf(image_tipdialog_bg_image,sizeof(image_tipdialog_bg_image),_("%sselect_warn.png"),ANTIVIRUS_IMAGE_DIR);
    tip_image = gtk_image_new_from_file(image_tipdialog_bg_image);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),tip_image,30,80);
    gtk_widget_show(tip_image);
    tip_label = gtk_label_new("");
    gtk_widget_show(tip_label);
    gtk_label_set_text(tip_label,warnMessage);
    change_font_style((GtkLabel *)tip_label,"<span font_desc='11'>%s</span>");
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),tip_label,110,90);
    sure_button = gtk_button_new();
    gtk_button_set_relief((GtkButton *)sure_button,GTK_RELIEF_NONE);
    memset(image_tipdialog_bg_image,0,sizeof(image_tipdialog_bg_image));
    snprintf(image_tipdialog_bg_image,sizeof(image_tipdialog_bg_image),_("%sdetermine.png"),ANTIVIRUS_IMAGE_DIR);
    sure_button_image = gtk_image_new_from_file(image_tipdialog_bg_image);
    gtk_button_set_image(sure_button,sure_button_image);
    if((strcmp(warnMessage,_("save sucess")))==0||(strcmp(warnMessage,_("sure-to-quit")))==0||(strcmp(warnMessage,_("Engine is initing...\nWait for a moment")))==0||(strcmp(warnMessage,_("Are you sure to stop scaning?")))==0||(strcmp(warnMessage,_("file exist,sure to cover")))==0)
    {
	//    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),sure_button,120,150);
	  gtk_layout_put(GTK_LAYOUT(tipdialog_layout),sure_button,210,150);
	    
    }
    else if (strcmp(warnMessage,_("this deal will deal thread from one file!"))==0)
    {
	gtk_layout_put(GTK_LAYOUT(tipdialog_layout),sure_button,180,150);
    }
    else
    {
        gtk_layout_put(GTK_LAYOUT(tipdialog_layout),sure_button,270,190);
    }
    gtk_widget_show(sure_button);
    g_signal_connect(G_OBJECT(sure_button),"clicked",G_CALLBACK(save_tipdialog),warnMessage);

    cancel_button = gtk_button_new();
    gtk_button_set_relief((GtkButton *)cancel_button,GTK_RELIEF_NONE);
    memset(image_tipdialog_bg_image,0,sizeof(image_tipdialog_bg_image));
    snprintf(image_tipdialog_bg_image,sizeof(image_tipdialog_bg_image),_("%scancel.png"),ANTIVIRUS_IMAGE_DIR);
    cancel_button_image = gtk_image_new_from_file(image_tipdialog_bg_image);
    gtk_button_set_image(cancel_button,cancel_button_image);
    gtk_layout_put(GTK_LAYOUT(tipdialog_layout),cancel_button,210,150);
    if (strcmp(warnMessage,_("this deal will deal thread from one file!"))==0)
    {
    	gtk_layout_put(GTK_LAYOUT(tipdialog_layout),cancel_button,270,150);
//    	gtk_layout_put(GTK_LAYOUT(tipdialog_layout),cancel_button,180,150);
	
    }
    else
    {
        //gtk_layout_put(GTK_LAYOUT(tipdialog_layout),cancel_button,210,150);
        gtk_layout_put(GTK_LAYOUT(tipdialog_layout),cancel_button,120,150);
    }
    if(dailog_type==2)
            gtk_widget_show(cancel_button);
    g_signal_connect(G_OBJECT(cancel_button),"clicked",G_CALLBACK(destroy_tipdialog),warnMessage);
    gtk_container_add(GTK_CONTAINER(tip_dialog),tipdialog_layout);
    gtk_widget_show(tipdialog_layout);
    gtk_widget_show(tip_dialog);


}



gint initWarn(GtkWidget *widget,gpointer data){
	int avlStatus = 0;
   char upgrade_dir[512];
           snprintf(upgrade_dir,sizeof(upgrade_dir),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
   avlStatus = get_upgrde_status(upgrade_dir);
   if(avlStatus == 6)
   {
	show_warn_info(_("Waiting for engine upgrade..."),1);
   }
   else
   {
	show_warn_info(_("Engine is initing...\nWait for a moment"),1);
   }
}


void change_font_style(GtkLabel *label,char *cssInfo)
{
	char *markup;
	const gchar *labelTitel = gtk_label_get_text(label);
	markup = g_markup_printf_escaped (cssInfo, labelTitel);
	gtk_label_set_markup (GTK_LABEL (label), markup);
	g_free (markup);
}



void waitScanThreadCacelFinish(pthread_t cancelThreadId)
{
        void *status;
        pthread_join(cancelThreadId, &status);
}

void print_filename(GtkWidget *file)
{
	gchar *filename;
    	filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
    	g_print("%s\n",filename);
    	gtk_widget_destroy(file);
}

void updting(GtkWidget *widget,gpointer data)
{
	gtk_widget_show(updatelabel);
}

void avlInitThread()
{
        int retEngineStatus = 0;
        int avlStatus = 0;
        char upgrade_dir[512];
        snprintf(upgrade_dir,sizeof(upgrade_dir),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
        write_antivirus_status(upgrade_dir,"1");
	clamav_init();
    	return ;
}

void hide_other_page()
{
    if(area==1)
    {
        gtk_widget_hide(feedback_label_button);
        gtk_widget_hide(check_update_button);
        gtk_widget_hide(table1);
        gtk_widget_hide(layout_main);
        gtk_widget_hide(title_layout);
        gtk_widget_hide(versionimage);
        gtk_widget_show(feedback_label_button_white_bg);
        gtk_widget_show(check_update_button_white_bg);

    }
    if(area==2)
    {
        gtk_widget_hide(feedback_label_button);
        gtk_widget_hide(check_update_button);
        gtk_widget_hide(table2);
        gtk_widget_hide(layout_main);
        gtk_widget_hide(title_layout);
        gtk_widget_hide(versionimage);
        gtk_widget_show(feedback_label_button_white_bg);
        gtk_widget_show(check_update_button_white_bg);
        free_devide_file_list_memory();
        free_white_list_file_memory();
    }
    if(area==3)
    {
        gtk_widget_hide(feedback_label_button);
        gtk_widget_hide(check_update_button);
        gtk_widget_hide(table3);
        gtk_widget_hide(layout_main);
        gtk_widget_hide(title_layout);
        gtk_widget_hide(versionimage);
        gtk_widget_show(feedback_label_button_white_bg);
        gtk_widget_show(check_update_button_white_bg);
    }
    if(area==4)
    {
        gtk_widget_hide(feedback_label_button);
        gtk_widget_hide(check_update_button);
        gtk_widget_hide(table4);
        gtk_widget_hide(layout_main);
        gtk_widget_hide(title_layout);
        gtk_widget_hide(versionimage);
        gtk_widget_show(feedback_label_button_white_bg);
        gtk_widget_show(check_update_button_white_bg);
    }
    if(area==5)
    {
        gtk_widget_hide(feedback_label_button);
        gtk_widget_hide(check_update_button);
        gtk_widget_hide(layout_main);
        gtk_widget_hide(title_layout);
        gtk_widget_hide(versionimage);
        gtk_widget_show(feedback_label_button_white_bg);
        gtk_widget_show(check_update_button_white_bg);
    }
    if(area == 6)
    {
        if(detail_result_table!=NULL)
            gtk_widget_hide(detail_result_table);
    }
    if(area==7)
    {
        gtk_widget_destroy(table7);
    }


}


/***将界面从主界面切换到扫描过程界面 *****/
void page_switch_to_scan()
{char image_quickscan_process_title[512];
           snprintf(image_quickscan_process_title,sizeof(image_quickscan_process_title),_("%squickscan_process_title.png"),ANTIVIRUS_IMAGE_DIR);

    hide_other_page();

    if(detail_result_table!=NULL)
    {
        gtk_widget_hide(detail_result_table);
    }
    if(scan_process_title_image != NULL )
    {
        if( scanInfo.scanType == FAST_SCAN)
        {
            gtk_image_set_from_file(scan_process_title_image,image_quickscan_process_title);

        }
        else if( scanInfo.scanType == ALL_SCAN)
        {   memset(image_quickscan_process_title,0,sizeof(image_quickscan_process_title));
            snprintf(image_quickscan_process_title,sizeof(image_quickscan_process_title),_("%sallscan_process_title.png"),ANTIVIRUS_IMAGE_DIR);
            gtk_image_set_from_file(scan_process_title_image,image_quickscan_process_title);

        }
        else if( scanInfo.scanType == CUSTOM_SCAN  || scanInfo.scanType == CLICK_DIR_SCAN || scanInfo.scanType == CLICK_FILE_SCAN )
        {memset(image_quickscan_process_title,0,sizeof(image_quickscan_process_title));
            snprintf(image_quickscan_process_title,sizeof(image_quickscan_process_title),_("%sselfscan_process_title.png"),ANTIVIRUS_IMAGE_DIR);
            gtk_image_set_from_file(scan_process_title_image,image_quickscan_process_title);

        }

        else if( scanInfo.scanType == SPECIFIED_SCAN)
        {memset(image_quickscan_process_title,0,sizeof(image_quickscan_process_title));
            snprintf(image_quickscan_process_title,sizeof(image_quickscan_process_title),_("%smediascan_process_title.png"),ANTIVIRUS_IMAGE_DIR);
            gtk_image_set_from_file(scan_process_title_image,image_quickscan_process_title);

        }
        else if( scanInfo.scanType == FIREFOX_DOWNLOAD_SCAN)
        {memset(image_quickscan_process_title,0,sizeof(image_quickscan_process_title));    
            snprintf(image_quickscan_process_title,sizeof(image_quickscan_process_title),_("%sdownloadscan_process_title.png"),ANTIVIRUS_IMAGE_DIR);
            gtk_image_set_from_file(scan_process_title_image,image_quickscan_process_title);

        }

    }
	if(area==10){
	printf("已经存在实时防护的界面了\n");
	gtk_widget_hide(real_protect_result_table);
}
    gtk_widget_show(table6);
    if (scanInfo.scanType == SPECIFIED_SCAN)
    {
         gtk_widget_hide(back_to_mainmenu_button);
         gtk_status_icon_set_has_tooltip (trayIcon,TRUE);
         gtk_status_icon_set_tooltip_text (trayIcon,_("scanning in usb device"));
         memset(image_quickscan_process_title,0,sizeof(image_quickscan_process_title));
            snprintf(image_quickscan_process_title,sizeof(image_quickscan_process_title),_("%sicon-scanning.gif"),ANTIVIRUS_IMAGE_DIR);

         gtk_status_icon_set_from_file (trayIcon,image_quickscan_process_title);
	 gtk_status_icon_set_visible(trayIcon,TRUE);
	 notify_pop(_("anti-virus"),_("scanning in usb device"),"/usr/share/icons/COS-desktop/status/scalable/drive-harddisk-symbolic.svg");

    }
    if(scanInfo.scanType == FIREFOX_DOWNLOAD_SCAN)
    {
         gtk_widget_hide(back_to_mainmenu_button);
         gtk_status_icon_set_has_tooltip (trayIcon,TRUE);
         gtk_status_icon_set_tooltip_text (trayIcon,_("scanning in firefox download file"));
         memset(image_quickscan_process_title,0,sizeof(image_quickscan_process_title));
            snprintf(image_quickscan_process_title,sizeof(image_quickscan_process_title),_("%sicon-scanning.gif"),ANTIVIRUS_IMAGE_DIR);

         gtk_status_icon_set_from_file (trayIcon,image_quickscan_process_title);
         memset(image_quickscan_process_title,0,sizeof(image_quickscan_process_title));
            snprintf(image_quickscan_process_title,sizeof(image_quickscan_process_title),_("%sfirefox.png"),ANTIVIRUS_IMAGE_DIR);
         notify_pop(_("anti-virus"),_("scanning in firefox download file"),image_quickscan_process_title);

    }
    area = 6;
}


//设置杀毒软件语言
void set_antivirus_language()
{
    //指定杀毒软件语言与系统语言一致
    setlocale(LC_ALL,"");

    //指定汉化文件名
    textdomain("antivirus");

}
gboolean check_current_user_autostart_set()
{
	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
    int is_boot_start = 0;
	is_boot_start = get_antivirus_setting_item_inttype("boot-start",set_file_path);
	printf("is_boot_start is :%d\n",is_boot_start);
    if(is_boot_start)
	{
	    return TRUE;
	}
		return FALSE;
}

int main(int argc,char *argv[])
{
	char delete_clamd_logfile[1024];

         if(argc >= 2)
	{
		if(strcmp(argv[1],"autostart")== 0)
		{
            AutoStartFirstShowFlag = 1;
		}
	}

	
	printf("argv[2] is :%s\n",argv[2]);
        if(argc >= 3 )
        {
            if(strcmp(argv[2],"") != 0)
            {
                 snprintf(g_current_user,sizeof(g_current_user),"%s",argv[2]);
            }
        }

    	
	snprintf(delete_clamd_logfile,sizeof(delete_clamd_logfile),"rm -rf %s%s_clamd.log",ANTIVIRUS_LOG_DIR,g_current_user);
	system(delete_clamd_logfile);
	system("rm /tmp/.firefox_info");
	system("rm  -rf /tmp/firefox_download*");
	system("/var/log/clamav/freshclam.log");

        char delete_status_cmd[512];
        snprintf(delete_status_cmd,sizeof(delete_status_cmd),"rm -rf %savlstatus.txt",ANTIVIRUS_TEMP_DIR);
        system(delete_status_cmd);


	char write_run_anti_flag_file[512];
	snprintf(write_run_anti_flag_file,sizeof(write_run_anti_flag_file),"echo 1 >%s%s_run_anti_flag_file",ANTIVIRUS_CONF_DIR,g_current_user);
	system(write_run_anti_flag_file);


	char tmp_scan_results[512];
	snprintf(tmp_scan_results,sizeof(tmp_scan_results),"rm -rf %sscan_results.txt",ANTIVIRUS_TEMP_DIR);
	system(tmp_scan_results);

	void *status; 
   	int ret;
/*
        if(!g_thread_supported())
        {
            g_thread_init(NULL);
        }
        gdk_threads_init();
*/

        gtk_init(&argc,&argv);


    
//		snprintf(g_current_user,sizeof(g_current_user),"%s","nfs");
    if(AutoStartFirstShowFlag == 1)
	{
		printf("in autostartflag\n");
	    if(!check_current_user_autostart_set())
		{
		    return 0;
		}
	}

        //创建病毒库初始化线程 
	if(AutoStartFirstShowFlag == 0)
	{
        ret = pthread_create(&avlInitThreadId,NULL,(void *)avlInitThread,NULL);
        if(ret != 0)
        {
            printf("create init thread error\n");
            return 1;
        }
	}

	if(access(tmp_scan_results,0)==0)
        {
                system(tmp_scan_results);
        }

//	system(">/tmp/.firefox_info");
        //设置杀毒软件语言
        set_antivirus_language();        

        //创建杀毒软件主界面
        if(argc >= 2)
        {
            if(strcmp(argv[1],"autostart")== 0)
            {
                create_home_page("autostart");
            }
            else
            {
                create_home_page("not-autostart");
            }
        }
        else
        {
            create_home_page("not-autostart");
        }

        printf("create home page over\n");

        //创建等待病毒库初始化结束线程
		if(AutoStartFirstShowFlag == 0)
		{
    	    ret = pthread_create(&waitInitThreadId,NULL,(void *)waitInitThread,attentionlabel);
    	    if(ret != 0)
    	    {
       		    printf("create thread error\n");
       		    return 1;
    	    }

		}

        g_thread_new("usb_detect",thread,NULL);

        gdk_threads_enter();

    	gtk_main();
       
        gdk_threads_leave();

		if(AutoStartFirstShowFlag == 0)
        {
    	    pthread_cancel(avlInitThreadId);
    	    pthread_join(avlInitThreadId,&status);
		}

    	freeGlist();
        //结束引擎
        cl_engine_free(engine);
    	return 0;

}
 int  get_scanType()
{
	return   scanInfo.scanType;
}

