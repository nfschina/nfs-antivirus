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
#include<gtk/gtk.h>
#include <time.h>
#include <string.h>
#include <libintl.h>
#include <locale.h>
#include <stdlib.h>
#include "result.h"
#include "resultcallbackFunc.h"
#include "result_detail.h"
#include "scan.h"
#include "./page/setting-page.h"
#include "./page/home-page.h"
#include "function.h"
#define _(STRING) gettext(STRING)
pthread_t auto_deal_ThreadId;
GtkWidget *auto_deal_dialog;
GtkWidget *warning_win;
extern GtkStatusIcon *trayIcon;
extern GtkWidget *back_to_mainmenu_button;
extern GtkWindow *window;//主窗口指针
extern char g_current_user[256]; //当前启动杀毒软件用户

void quit_auto_deal(GtkWidget *widget,gpointer *data)
{char tmp_exit_auto_deal[512];
           snprintf(tmp_exit_auto_deal,sizeof(tmp_exit_auto_deal),"echo 1 >%sexit_auto_deal.txt",ANTIVIRUS_TEMP_DIR);

        system(tmp_exit_auto_deal);

}

gboolean is_has_threat()
{char tmp_scan_results[512];
           snprintf(tmp_scan_results,sizeof(tmp_scan_results),"%sscan_results.txt",ANTIVIRUS_TEMP_DIR);

	if(!access(tmp_scan_results,0))
		return TRUE;
	else
		return FALSE;
}

void exec_auto_handle();
void yes_callbackfunc(GtkWidget *widget,gpointer data)
{
        gtk_widget_destroy(warning_win);
        if(auto_deal_dialog == NULL)
        {
                auto_deal_page();
        }
        else
        {
                gtk_widget_show(auto_deal_dialog);
        }
 
        usleep(5);

        g_thread_new("auto_handle",exec_auto_handle,NULL);

}
void no_callbackfunc(GtkWidget *widget,gpointer data)
{
        gtk_widget_destroy(warning_win);
        page_switch_to_result_detail(data,1,1,NULL,NULL);
}
void warning_dialog(int cancel_var)
{char image_shadu[512];
           snprintf(image_shadu,sizeof(image_shadu),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);

        GtkWidget *warning_vbox,*warning_fix,*warning_label,*yes_button,*no_button,*warning_image,*yes_button_image,*no_button_image;
	
	GtkWidget *autodeal_warning_layout,*autodeal_title_image,*autodeal_title_quit_button,*autodeal_quit_image;
        warning_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(warning_win,_("warning"));
	gtk_window_set_decorated(GTK_WINDOW(warning_win), FALSE);
        gtk_window_set_position(GTK_WINDOW(warning_win),GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(warning_win),380,248);
        gtk_window_set_icon_from_file(GTK_WINDOW(warning_win),image_shadu,NULL);
        gtk_widget_set_events(GTK_WINDOW(warning_win),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

        g_signal_connect(GTK_WINDOW(warning_win), "button_press_event", G_CALLBACK (button_press_event), warning_win);       // 加入事件回调
//        g_signal_connect(GTK_WINDOW(warning_win), "motion_notify_event",G_CALLBACK( motion_notify_event), warning_win);
        g_signal_connect(GTK_WINDOW(warning_win), "button_release_event", G_CALLBACK(button_release_event), warning_win);

	gtk_window_set_transient_for(warning_win,window);
	gtk_window_set_modal(warning_win,TRUE);
	gtk_window_set_position(warning_win,GTK_WIN_POS_CENTER_ALWAYS);
        gtk_widget_show(warning_win);

	autodeal_warning_layout=gtk_layout_new(NULL, NULL);
	gtk_widget_show(autodeal_warning_layout);
        gtk_widget_set_size_request(autodeal_warning_layout,380,248);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%sautodeal_warn_bg.png"),ANTIVIRUS_IMAGE_DIR);
	autodeal_title_image=gtk_image_new_from_file(image_shadu);
	gtk_widget_show(autodeal_title_image);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),autodeal_title_image, 0, 0);
         char css_tipdialog[512];
        snprintf(css_tipdialog,sizeof(css_tipdialog),"%stipdialog.css",ANTIVIRUS_CSS_DIR);
	GFile *file=g_file_new_for_path(css_tipdialog);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

	autodeal_title_quit_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)autodeal_title_quit_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(autodeal_title_quit_button,24,24);
        gtk_widget_show(autodeal_title_quit_button);
        g_signal_connect(G_OBJECT(autodeal_title_quit_button),"clicked",G_CALLBACK(no_callbackfunc),warning_win);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
        autodeal_quit_image= gtk_image_new_from_file(image_shadu);
        gtk_widget_show(autodeal_quit_image);
        gtk_button_set_image(autodeal_title_quit_button,autodeal_quit_image);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout), autodeal_title_quit_button, 360, 0);
	gtk_widget_set_name(autodeal_title_quit_button,"tipdialog-close");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%squitupdate_icon.png"),ANTIVIRUS_IMAGE_DIR);
	warning_image = gtk_image_new_from_file(image_shadu);
        gtk_widget_show(warning_image);
	gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),warning_image, 36, 102);

	warning_label = gtk_label_new(_("There are some wrong with your network or apt-tools,which will affects the result,are you sure to continue to the opration?"));
	gtk_widget_show(warning_label);
	gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),warning_label, 115, 106);
	change_font_style(warning_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");

	yes_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)yes_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(yes_button,82,34);
        gtk_widget_show(yes_button);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%squitupgrade_OK.png"),ANTIVIRUS_IMAGE_DIR);
	yes_button_image=gtk_image_new_from_file(image_shadu);
        gtk_widget_show(yes_button_image);
        gtk_button_set_image(yes_button,yes_button_image);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),yes_button, 190, 192);

	no_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)no_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(no_button,82,34);
        gtk_widget_show( no_button);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%squitupgrade_NO.png"),ANTIVIRUS_IMAGE_DIR);
        no_button_image=gtk_image_new_from_file(image_shadu);
        gtk_widget_show(no_button_image);
        gtk_button_set_image(no_button,no_button_image);
        gtk_layout_put(GTK_LAYOUT(autodeal_warning_layout),no_button, 280, 192);


	gtk_container_add(GTK_CONTAINER(warning_win),autodeal_warning_layout);
        g_signal_connect(G_OBJECT(yes_button),"clicked",G_CALLBACK(yes_callbackfunc),cancel_var);
        g_signal_connect(G_OBJECT(no_button),"clicked",G_CALLBACK(no_callbackfunc),cancel_var);
}

void auto_deal_page()
{
	GtkWidget *auto_deal_layout,*autodeal_title_image,*close_autodealwin_button,*close_autodealwin_image;
	GtkWidget *auto_deal_image,*isnewlabel;
        char image_shadu[512], css_tipdialog[512];
        snprintf(image_shadu,sizeof(image_shadu),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
        snprintf(css_tipdialog,sizeof(css_tipdialog),"%stipdialog.css",ANTIVIRUS_CSS_DIR);
        auto_deal_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(auto_deal_dialog,_("auto handing"));
        gtk_window_set_transient_for(auto_deal_dialog,window);
        gtk_window_set_modal(auto_deal_dialog,TRUE);
	gtk_window_set_decorated(GTK_WINDOW(auto_deal_dialog), FALSE);
        gtk_window_set_position(GTK_WINDOW(auto_deal_dialog),GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(auto_deal_dialog),320,246);
        gtk_window_set_icon_from_file(GTK_WINDOW(auto_deal_dialog),image_shadu,NULL);
        gtk_widget_set_events(GTK_WINDOW(auto_deal_dialog),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

        g_signal_connect(GTK_WINDOW(auto_deal_dialog), "button_press_event", G_CALLBACK (button_press_event), auto_deal_dialog);       // 加入事件回调
//        g_signal_connect(GTK_WINDOW(auto_deal_dialog), "motion_notify_event",G_CALLBACK( motion_notify_event), auto_deal_dialog);
        g_signal_connect(GTK_WINDOW(auto_deal_dialog), "button_release_event", G_CALLBACK(button_release_event), auto_deal_dialog);

	auto_deal_layout = gtk_layout_new(NULL, NULL);
        gtk_widget_set_size_request(auto_deal_layout,320,246);
        gtk_widget_show(auto_deal_layout);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%sautodeal_bg.png"),ANTIVIRUS_IMAGE_DIR);
	autodeal_title_image = gtk_image_new_from_file(image_shadu);
        gtk_layout_put(GTK_LAYOUT(auto_deal_layout),autodeal_title_image, 0, 0);
        gtk_widget_show(autodeal_title_image);
        
	GFile *file=g_file_new_for_path(css_tipdialog);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

	close_autodealwin_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)close_autodealwin_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(close_autodealwin_button,24,24);
        gtk_widget_show(close_autodealwin_button);
        g_signal_connect(G_OBJECT(close_autodealwin_button),"clicked",G_CALLBACK(quit_auto_deal),auto_deal_dialog);
        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
        close_autodealwin_image= gtk_image_new_from_file(image_shadu);
        gtk_widget_show(close_autodealwin_image);
        gtk_button_set_image(close_autodealwin_button,close_autodealwin_image);
        gtk_layout_put(GTK_LAYOUT(auto_deal_layout),close_autodealwin_button, 293, 0);
	gtk_widget_set_name(close_autodealwin_button,"tipdialog-close");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);

        memset(image_shadu,0,sizeof(image_shadu));
        snprintf(image_shadu,sizeof(image_shadu),_("%sdeal-file.gif"),ANTIVIRUS_IMAGE_DIR);
	auto_deal_image=gtk_image_new_from_file(image_shadu);
        gtk_layout_put(GTK_LAYOUT(auto_deal_layout),auto_deal_image, 112, 75);
        isnewlabel =(GtkLabel *) gtk_label_new(_("system is auto dealing with threats"));
        gtk_widget_show(auto_deal_image);
        change_font_style(isnewlabel,"<span foreground=\"#686868\" font_desc='11'>%s</span>");
        gtk_widget_show(isnewlabel);
        gtk_layout_put(GTK_LAYOUT(auto_deal_layout),isnewlabel, 100, 180);
	
	gtk_container_add(GTK_CONTAINER(auto_deal_dialog),auto_deal_layout);
	
        gtk_widget_show(auto_deal_dialog);
}


int get_auto_handle()
{
    int auto_handle_flag;
	char set_file_path[1024];
	snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
    auto_handle_flag=get_antivirus_setting_item_inttype("anti-deal-style",set_file_path);
    return auto_handle_flag;
}
void * check_apt_tool_func(char *resultBuffer)
{
        FILE *pBufSize;
        int sumsize = 0;
        char cmd[1024];
	int length=1024;

        snprintf(cmd,sizeof(cmd),"apt-get update > /dev/null 2>&1 ;echo $?");
        if((pBufSize = popen(cmd,"r")) == NULL)
        {
            printf("error\n");
            return ;
        }
        fgets(resultBuffer,length,pBufSize);
        fclose(pBufSize);
}
void auto_handle_param(char * resultBuffer, int length)
{
        FILE *pBufSize;
        int sumsize = 0;
        char cmd[1024];
	char devidefile_dir_info[256];

	    char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"/usr/share/antivirus/config/%s_setting.txt",g_current_user);

        get_antivirus_setting_item_stringtype("devide-dir",devidefile_dir_info,set_file_path);
        
	snprintf(cmd,sizeof(cmd),"bash %sauto_handle.sh %s %s\n",ANTIVIRUS_BIN_AUTO_HANDLE_DIR,devidefile_dir_info,set_file_path);

	if((pBufSize = popen(cmd,"r")) == NULL)
        {
            return ;
        }
        fgets(resultBuffer,length,pBufSize);
        fclose(pBufSize);

}
void exec_auto_handle()
{
	printf("222222222222222 in exec auto handle \n");
        char resultBuffer[1024] = "";
        auto_handle_param(resultBuffer,1024);
	char *delete_num = NULL ,*repair_num = NULL ;
        if(strcmp(resultBuffer,"") != 0)
        {
            delete_num=strtok(resultBuffer," ");
        }
        char del_num[100] = "" ,rep_num[100] = "";
        if(delete_num !=NULL)
        {
            repair_num=strtok(NULL," ");
            strncpy(del_num,delete_num,sizeof(del_num));
            strncpy(rep_num,repair_num,sizeof(rep_num));
        }
        
        gdk_threads_enter();

	printf("ccccccccccccccc in auto deal dialog \n");
     
        if(gtk_widget_get_visible(auto_deal_dialog))
        {
            gtk_widget_hide(auto_deal_dialog);
        }
	printf("dddddddddddddddddd in auto deal dialog \n");


        page_switch_to_result_detail(0,1,2,del_num,rep_num);

	printf("eeeeeeeeeeeeeee in auto deal dialog \n");

        gdk_threads_leave();
}

void how_switch_to_result_detail(int cancel)
{       char image_NFSantivirus[512];

        pthread_mutex_t mut_buf=PTHREAD_MUTEX_INITIALIZER;

        snprintf(image_NFSantivirus,sizeof(image_NFSantivirus),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
	gtk_status_icon_set_has_tooltip (trayIcon,TRUE);
        gtk_status_icon_set_tooltip_text (trayIcon,_("anti-virus"));

                
                gtk_status_icon_set_from_file(trayIcon,image_NFSantivirus);
	if(get_scanType()==SPECIFIED_SCAN || get_scanType()== FIREFOX_DOWNLOAD_SCAN )
        {
                gtk_widget_show(back_to_mainmenu_button);
		if(get_scanType()==SPECIFIED_SCAN)
		{
			if(is_has_threat())
			{
				if(get_auto_handle() == AUTO_DEAL)
				
        {memset(image_NFSantivirus,0,sizeof(image_NFSantivirus));
        snprintf(image_NFSantivirus,sizeof(image_NFSantivirus),_("%supdate_warn.png"),ANTIVIRUS_IMAGE_DIR);
       notify_pop(_("anti-virus"),_("usb device scan complete,auto deal already deal with the threads for you!"),image_NFSantivirus);}
				else

            {memset(image_NFSantivirus,0,sizeof(image_NFSantivirus));
            snprintf(image_NFSantivirus,sizeof(image_NFSantivirus),_("%supdate_warn.png"),ANTIVIRUS_IMAGE_DIR);
					notify_pop(_("anti-virus"),_("usb device scan complete,found threads,please press the antivirus button to deal!"),image_NFSantivirus);}
			}
			else
			{
                         memset(image_NFSantivirus,0,sizeof(image_NFSantivirus));
                            snprintf(image_NFSantivirus,sizeof(image_NFSantivirus),_("%supdate_success.png"),ANTIVIRUS_IMAGE_DIR);
				notify_pop(_("anti-virus"),_("usb device scan complete,no thread found!"),image_NFSantivirus);
			}
		}
		if(get_scanType()== FIREFOX_DOWNLOAD_SCAN)
		{
                        if(is_has_threat())
                        {
                                if(get_auto_handle() == AUTO_DEAL)
                    { memset(image_NFSantivirus,0,sizeof(image_NFSantivirus));
                            snprintf(image_NFSantivirus,sizeof(image_NFSantivirus),_("%supdate_warn.png"),ANTIVIRUS_IMAGE_DIR);
                                        notify_pop(_("anti-virus"),_("download of firefox scan complete,auto deal already deal with the threads for you!"),image_NFSantivirus);}
                                else
                                        notify_pop(_("anti-virus"),_("download of firefox scan complete,found threads,please press the antivirus button to deal!"),image_NFSantivirus);
                        }
                        else
                        {memset(image_NFSantivirus,0,sizeof(image_NFSantivirus));
                            snprintf(image_NFSantivirus,sizeof(image_NFSantivirus),_("%supdate_success.png"),ANTIVIRUS_IMAGE_DIR);
                                notify_pop(_("anti-virus"),_("download of firefox scan complete,no thread found!"),image_NFSantivirus);
                        }
                }

        }

	if(is_has_threat())
        {
		
        	if(get_auto_handle() == AUTO_DEAL)
		{
			char apt_resultBuffer[1024];
                        int timing_pthread;
                        pthread_t timing_pthreadId;

/*                        warning_dialog(cancel);

                        timing_pthread = pthread_create(&timing_pthreadId,NULL,(void *)check_apt_tool_func,apt_resultBuffer);
                        if(timing_pthread != 0)
                        {
                                exit(1);
                        }
                        sleep(30);
                        pthread_cancel(timing_pthread);
                        if (strcmp (apt_resultBuffer,"0\n")!=0 )
                        {
                        	//warning_dialog(cancel);
			}
			else
                        {
                                gtk_widget_destroy(warning_win);*/
                               /* if(auto_deal_dialog == NULL)
                                {
                                        gdk_threads_enter();
                                        
                                        pthread_mutex_lock(&mut_buf);

                                        auto_deal_page();

                                        pthread_mutex_unlock(&mut_buf);

                                        gdk_threads_leave();

                                }
                                else
                                {
                                        gdk_threads_enter();

                                        pthread_mutex_lock(&mut_buf);

                                        gtk_widget_show(auto_deal_dialog);
 
                                        pthread_mutex_unlock(&mut_buf);

                                        gdk_threads_leave();


                                }

                                usleep(5);*/
			        printf("in aaaaaaaaaaaaaaaa exec auto handle \n");

                                g_thread_new("auto_handle",exec_auto_handle,NULL);
//                        }

		}
                else
			page_switch_to_result_detail(cancel,1,1,NULL,NULL);
        }
        else
              page_switch_to_result_detail(cancel,0,0,NULL,NULL);
}
