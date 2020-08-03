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
#include <stdlib.h>
#include "upgrade-page.h"
#include "../upgradeFunction.h"
#include "../page/home-page.h"
#include "../function.h"
#include "upgrade-page.h"
#define _(STRING) gettext(STRING)

char upgradeBeginTime[30];
int isnewversion = 1;
pthread_t avlUpgradeRefreshThreadId,avlUpgradeRunThreadId;
GtkWidget *dialog;
 GtkWidget *isnewlabel;
 GtkWidget *update_status_image,*fix,*update_layout;
extern GtkWidget *last_update_label;
extern GtkWidget *window;
extern int clamav_version_label_button_status;
extern int clamav_thread_label_button_status;
//extern GtkLabel * attentionlabel;
extern int avlInit;
extern char g_current_user[256]; //当前启动杀毒软件用户
void derect_quit_upgrade();
void quitupgrade_warn_dialog(GtkWidget *widget,gpointer data);
void quitupgrade_select_NO(GtkWidget *widget,gpointer data);
void quitupgrade_select_OK(GtkWidget *widget,gpointer data);
void show_or_hide_net_label(GtkWidget *widget,gpointer data){
	if(gtk_widget_get_visible(data)){
		char image_dir[256];
                snprintf(image_dir,sizeof(image_dir),_("%supdating_button_image.png"),ANTIVIRUS_IMAGE_DIR);
		gtk_button_set_image(GTK_BUTTON(update_net_botton),gtk_image_new_from_file(image_dir));
		gtk_widget_hide(updating_net_check_fix);
		gtk_widget_hide(data);
	}
	else{
		char image_dir[256];
                snprintf(image_dir,sizeof(image_dir),_("%supdating_button_hide_image.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_button_set_image(GTK_BUTTON(update_net_botton),gtk_image_new_from_file(image_dir));
		gtk_widget_show(updating_net_check_fix);
		gtk_widget_show(data);
	}
}
void show_or_hide_clamav_version_label(GtkWidget *widget,gpointer data){
	char status_temp[1024];
	snprintf(status_temp,sizeof(status_temp),"%savlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR);
        if(gtk_widget_get_visible(clamav_now_version)){
		char image_dir[256];
                snprintf(image_dir,sizeof(image_dir),_("%supdating_button_image.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_button_set_image(GTK_BUTTON(clamav_button),gtk_image_new_from_file(image_dir));
		gtk_widget_hide(clamav_version_fix);
                gtk_widget_hide(clamav_now_version);
		gtk_widget_hide(clamav_update_success);
		clamav_now_version_label_status = 1;
                clamav_update_success_lable_status = 1;
        }
        else{
		char image_dir[256];
                snprintf(image_dir,sizeof(image_dir),_("%supdating_button_hide_image.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_button_set_image(GTK_BUTTON(clamav_button),gtk_image_new_from_file(image_dir));
		gtk_widget_show(clamav_version_fix);
                gtk_widget_show(clamav_now_version);
//		int iUpgradeStatus = get_upgrde_status(status_temp);
		//printf();
		printf("clamav version 第二个label是%d\n",clamav_version_label_button_status);
		if(clamav_version_label_button_status){
		gtk_widget_show(clamav_update_success);}
		clamav_now_version_label_status = 0;
                clamav_update_success_lable_status = 0;
        }
}
void show_or_hide_clamav_thread_update_label(GtkWidget *widget,gpointer data){
	char status_temp[1024];
        snprintf(status_temp,sizeof(status_temp),"%savlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR);
        if(gtk_widget_get_visible(clamav_thread_now_version)){
		char image_dir[256];
                snprintf(image_dir,sizeof(image_dir),_("%supdating_button_image.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_button_set_image(GTK_BUTTON(clamav_thread_button),gtk_image_new_from_file(image_dir));
		gtk_widget_hide(update_clamav_thread_fix);
                gtk_widget_hide(clamav_thread_now_version);
                gtk_widget_hide(clamav_thread_update_success_label);
		clamav_thread_now_version_label_status = 1;
                clamav_thread_update_success_label_status = 1;
        }
        else{
		char image_dir[256];
                snprintf(image_dir,sizeof(image_dir),_("%supdating_button_hide_image.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_button_set_image(GTK_BUTTON(clamav_thread_button),gtk_image_new_from_file(image_dir));
		gtk_widget_show(update_clamav_thread_fix);
		int iUpgradeStatus = get_upgrde_status(status_temp);
//		if(iUpgradeStatus==15||iUpgradeStatus==18){
                gtk_widget_show(clamav_thread_now_version);
		printf("clamav thread 第二个label%d\n",clamav_thread_label_button_status);
		if(clamav_thread_label_button_status){
                gtk_widget_show(clamav_thread_update_success_label);}
		clamav_thread_now_version_label_status = 0;
                clamav_thread_update_success_label_status = 0;
        }
}
//升级界面函数
void updatewindow(GtkWidget *widget,gpointer data)
{
	
	GtkWidget *quitupdate_button,*quitupdate_button_image;
	GtkWidget *update_title_image;
	char image_dir[256];
        dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(dialog,_("updating"));
        gtk_window_set_transient_for(dialog,window);
        gtk_window_set_modal(dialog,TRUE);
	gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
	gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(dialog),320,245);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
	gtk_window_set_icon_from_file(GTK_WINDOW(dialog),image_dir,NULL);
	gtk_widget_set_events(GTK_WINDOW(dialog),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

    	g_signal_connect(GTK_WINDOW(dialog), "button_press_event", G_CALLBACK (button_press_event), dialog);       // 加入事件回调
//    	g_signal_connect(GTK_WINDOW(dialog), "motion_notify_event",G_CALLBACK( motion_notify_event), dialog);
    	g_signal_connect(GTK_WINDOW(dialog), "button_release_event", G_CALLBACK(button_release_event), dialog);
	
	update_layout = gtk_layout_new(NULL, NULL);
	gtk_widget_show(update_layout);
	gtk_widget_set_size_request(update_layout,320,245);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%supdate_bg.png"),ANTIVIRUS_IMAGE_DIR);
	
	update_title_image = gtk_image_new_from_file(image_dir);
	gtk_widget_show(update_title_image);
	gtk_layout_put(GTK_LAYOUT(update_layout),update_title_image, 0, 0);

	char tipdialog_css[256];
        snprintf(tipdialog_css,sizeof(tipdialog_css),"%stipdialog.css",ANTIVIRUS_CSS_DIR);

	GFile *file=g_file_new_for_path(tipdialog_css);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);
	
	quitupdate_button=gtk_button_new();
	gtk_button_set_relief((GtkButton *)quitupdate_button,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(quitupdate_button,17,17);
	gtk_widget_show(quitupdate_button);
	g_signal_connect(G_OBJECT(quitupdate_button),"clicked",G_CALLBACK(quitupgrade),dialog);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
	quitupdate_button_image= gtk_image_new_from_file(image_dir);
	gtk_widget_show(quitupdate_button_image);
	gtk_button_set_image(quitupdate_button,quitupdate_button_image);
	gtk_widget_set_size_request(quitupdate_button,24,24);
	gtk_layout_put(GTK_LAYOUT(update_layout),quitupdate_button, 290, -5);	
	gtk_widget_set_name(quitupdate_button,"tipdialog-close");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
		
	updating_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_size_request(updating_box,322,202);
	gtk_layout_put(GTK_LAYOUT(update_layout),updating_box, 0, 47);	
	gtk_widget_show(updating_box);

	
/*	GtkWidget *updating_sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_set_size_request(updating_sw,322,202);
        gtk_scrolled_window_set_min_content_height((GtkScrolledWindow *)updating_sw,202);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(updating_sw), GTK_POLICY_ALWAYS,GTK_POLICY_ALWAYS);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(updating_sw), GTK_SHADOW_ETCHED_IN);
	gtk_widget_show(updating_sw);
	gtk_container_add(GTK_CONTAINER(updating_box),updating_sw);*/

	GtkWidget *updatint_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_size_request(updatint_content_box,322,202);
	gtk_box_pack_start(GTK_BOX (updating_box),updatint_content_box,FALSE,FALSE,10);
//	gtk_container_add(GTK_CONTAINER(updating_box),updatint_content_box);
	gtk_widget_show(updatint_content_box);

	GtkWidget *updating_net_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_widget_show(updating_net_box);
	gtk_box_pack_start(GTK_BOX (updatint_content_box),updating_net_box,FALSE,FALSE,0);
	
	
	updating_net_detail_label = gtk_label_new(_("net-is-ok"));
	updating_net_check_fix = gtk_fixed_new();
	gtk_fixed_put((GtkFixed *)updating_net_check_fix,updating_net_detail_label,40,0);
//      gtk_box_pack_start(GTK_BOX (updating_net_detail_box),updating_net_detail_label,FALSE,FALSE,50);
        gtk_box_pack_start(GTK_BOX (updatint_content_box),updating_net_check_fix,FALSE,FALSE,0);


	update_net_botton = gtk_button_new();
	gtk_button_set_relief((GtkButton *)update_net_botton,GTK_RELIEF_NONE);
	GtkWidget *updating_button_image = gtk_image_new_from_file("/usr/share/antivirus/images/updating_button_hide_image.png");
	gtk_widget_show(updating_button_image);
	gtk_button_set_image(GTK_BUTTON(update_net_botton),updating_button_image);
//	gtk_widget_show(update_net_botton);
	g_signal_connect (update_net_botton, "clicked", G_CALLBACK (show_or_hide_net_label),updating_net_detail_label);

	updating_net_check_label = gtk_label_new(_("Is upgrading, please wait a moment"));
	change_font_style(updating_net_check_label,"<span foreground=\"#000000\" font_desc='12'>%s</span>");
	gtk_widget_show(updating_net_check_label);
	updating_net_pass_label = gtk_label_new(_("pass"));
	change_font_style(updating_net_pass_label,"<span foreground=\"#000000\" font_desc='10'>%s</span>");
//	gtk_widget_show(updating_net_pass_label);
	gtk_box_pack_start(GTK_BOX (updating_net_box),update_net_botton,FALSE,FALSE,0);	
	gtk_box_pack_start(GTK_BOX (updating_net_box),updating_net_check_label,FALSE,FALSE,10);
	gtk_box_pack_end(GTK_BOX (updating_net_box),updating_net_pass_label,FALSE,FALSE,10);


	//GtkWidget *clamav_version_check_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	//gtk_widget_show(clamav_version_check_box);
//	gtk_box_pack_start(GTK_BOX (updatint_content_box),clamav_version_check_box,FALSE,FALSE,0);

	clamav_now_version = gtk_label_new(_("clamav-now_version"));
	clamav_version_fix = gtk_fixed_new();
	gtk_fixed_put((GtkFixed *)clamav_version_fix,clamav_now_version,40,0);
	clamav_update_success = gtk_label_new(_("clamav-now_version"));
	gtk_fixed_put((GtkFixed *)clamav_version_fix,clamav_update_success,40,15);
	gtk_box_pack_start(GTK_BOX (updatint_content_box),clamav_version_fix,FALSE,FALSE,0);

	clamav_button = gtk_button_new();
	gtk_button_set_relief((GtkButton *)clamav_button,GTK_RELIEF_NONE);
	GtkWidget *clamav_button_iamge = gtk_image_new_from_file("/usr/share/antivirus/images/updating_button_hide_image.png");
	gtk_widget_show(clamav_button_iamge);
	gtk_button_set_image(GTK_BUTTON(clamav_button),clamav_button_iamge);
//	gtk_widget_show(clamav_button);
	//gtk_box_pack_start(GTK_BOX (clamav_version_check_box),clamav_button,FALSE,FALSE,0);
//	g_signal_connect (clamav_button, "clicked", G_CALLBACK (show_or_hide_clamav_version_label),NULL);

//	clamav_version_check = gtk_label_new(_("clamav-version-check"));
//	change_font_style(clamav_version_check,"<span foreground=\"#000000\" font_desc='10'>%s</span>");
//	gtk_widget_show(clamav_version_check);
//	gtk_box_pack_start(GTK_BOX (clamav_version_check_box),clamav_version_check,FALSE,FALSE,10);

	clamav_version_pass = gtk_label_new(_("pass"));
	change_font_style(clamav_version_pass,"<span foreground=\"#000000\" font_desc='10'>%s</span>");
//	gtk_widget_show(clamav_version_pass);
//	gtk_box_pack_end(GTK_BOX (clamav_version_check_box),clamav_version_pass,FALSE,FALSE,10);

	GtkWidget *updating_thread_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_widget_show(updating_thread_box);
	gtk_box_pack_start(GTK_BOX (updatint_content_box),updating_thread_box,FALSE,FALSE,0);

	clamav_thread_now_version= gtk_label_new(_("clamav-thread-now-version"));
	update_clamav_thread_fix = gtk_fixed_new();
        gtk_fixed_put((GtkFixed *)update_clamav_thread_fix,clamav_thread_now_version,40,0);
	clamav_thread_update_success_label = gtk_label_new(_("clamav-thread-now-version"));
	gtk_fixed_put((GtkFixed *)update_clamav_thread_fix,clamav_thread_update_success_label,40,15);
	gtk_box_pack_start(GTK_BOX (updatint_content_box),update_clamav_thread_fix,FALSE,FALSE,0);
	

	clamav_thread_button = gtk_button_new();
	gtk_button_set_relief((GtkButton *)clamav_thread_button,GTK_RELIEF_NONE);
	GtkWidget *clamav_thread_image = gtk_image_new_from_file("/usr/share/antivirus/images/updating_button_hide_image.png");
	gtk_widget_show(clamav_thread_image);
	gtk_button_set_image(GTK_BUTTON(clamav_thread_button),clamav_thread_image);
//	gtk_widget_show(clamav_thread_button);
	gtk_box_pack_start(GTK_BOX (updating_thread_box),clamav_thread_button,FALSE,FALSE,0);
	g_signal_connect (clamav_thread_button, "clicked", G_CALLBACK (show_or_hide_clamav_thread_update_label),NULL);

	//updating_clamav_thread_label = gtk_label_new(_("update-clamav-thread"));
	//change_font_style(//updating_clamav_thread_label,"<span foreground=\"#000000\" font_desc='10'>%s</span>");
	//gtk_widget_show(updating_clamav_thread_label);
	//gtk_box_pack_start(GTK_BOX (updating_thread_box),updating_clamav_thread_label,FALSE,FALSE,10);
	
	updating_clamav_thread_pass_label = gtk_label_new(_("pass"));
	change_font_style(updating_clamav_thread_pass_label,"<span foreground=\"#000000\" font_desc='10'>%s</span>");
//	gtk_widget_show(updating_clamav_thread_pass_label);
	gtk_box_pack_end(GTK_BOX (updating_thread_box),updating_clamav_thread_pass_label,FALSE,FALSE,10);


	update_result_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_size_request(update_result_box,322,202);
	//gtk_widget_show(update_result_box);
        gtk_layout_put(GTK_LAYOUT(update_layout),update_result_box, 0, 47);
	
	update_fail_success_image = gtk_image_new_from_file("/usr/share/antivirus/images/update_success.png");
	gtk_widget_show(update_fail_success_image);
	GtkWidget *update_result_content_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_widget_show(update_result_content_box);
	gtk_box_pack_start(GTK_BOX (update_result_box),update_result_content_box,FALSE,FALSE,60);	
	
	update_net_result_label = gtk_label_new(_("network problems,upgrade fail"));
	change_font_style(update_net_result_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");
//	gtk_widget_show(update_net_result_label);
	update_pro_result_label = gtk_label_new(_("locked,upgrade fail"));
	change_font_style(update_pro_result_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");
//	gtk_widget_show(update_pro_result_label);
	gtk_box_pack_start(GTK_BOX (update_result_content_box),update_fail_success_image,FALSE,FALSE,30);
	gtk_box_pack_start(GTK_BOX (update_result_content_box),update_net_result_label,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX (update_result_content_box),update_pro_result_label,FALSE,FALSE,0);
//	gtk_fixed_put((GtkFixed *)updating_fix,update_net_botton,10,10);
/*	white_list_sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_set_size_request(white_list_sw,550,242);
        gtk_scrolled_window_set_min_content_height((GtkScrolledWindow *)white_list_sw,242);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(white_list_sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(white_list_sw), GTK_SHADOW_ETCHED_IN);*/
/*	if(isnewversion==0)
        {
		 gtk_layout_put(GTK_LAYOUT(update_layout),update_status_image, 124, 89);
		isnewlabel = (GtkLabel *)gtk_label_new(_("now-is-new"));
        }
        else
        {
		memset(image_dir,0,sizeof(image_dir));
        	snprintf(image_dir,sizeof(image_dir),_("%supdate_doing.gif"),ANTIVIRUS_IMAGE_DIR);
                update_status_image=gtk_image_new_from_file(image_dir);
		gtk_layout_put(GTK_LAYOUT(update_layout),update_status_image, 125, 75);
                isnewlabel =(GtkLabel *) gtk_label_new(_("scaning-please-wait"));
        }
        change_font_style(isnewlabel,"<span font_desc='11'>%s</span>");
	gtk_layout_put(GTK_LAYOUT(update_layout),isnewlabel, 115, 183);
	gtk_widget_show(isnewlabel);
        gtk_widget_show(update_status_image);	*/
	
	
	gtk_container_add(GTK_CONTAINER(dialog),update_layout);	

        time_t now;
        now = time(0);
        struct tm *tnow = localtime(&now);
        sprintf(upgradeBeginTime,"%04d/%02d/%02d %02d:%02d:%02d", 1900+tnow->tm_year,
             tnow->tm_mon+1,
             tnow->tm_mday,
             tnow->tm_hour,
             tnow->tm_min,
             tnow->tm_sec);
	char avlUpgradeStatus[256];
	char cmd[256];
        snprintf(cmd,sizeof(cmd),"rm %savlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR);

        system(cmd);
        int retUpgradeRefresh,retUpgradeRun;
        retUpgradeRefresh = pthread_create(&avlUpgradeRefreshThreadId,NULL,(void *)avlUpgradeLibRefresh,NULL);
        if(retUpgradeRefresh != 0)
        {
            exit(1);
        }
        retUpgradeRun = pthread_create(&avlUpgradeRunThreadId,NULL,(void *)avlUpgradeLibRun,NULL);
        if(retUpgradeRun != 0)
        {
            exit(1);
        }

	gtk_widget_show(dialog);
}

int quitupgrade_dialog_create(const gchar *str)
{
	GtkWidget *quitupgrade_dialog,*quitupgrade_layout,*quitupdate_bg_image,*quitupdate_dialog_button,*quitupgrade_button_image,*quitupgrade_icon_image,*quitupgrade_label,*quitupgrade_OK_button,*quitupgrade_NO_button,*quitupgrade_OK_image,*quitupgrade_NO_image;
	char image_dir[256];
        quitupgrade_dialog=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(quitupgrade_dialog,_("confirm quit update"));
        gtk_window_set_transient_for(quitupgrade_dialog,window);
        gtk_window_set_modal(quitupgrade_dialog,TRUE);
        gtk_window_set_decorated(GTK_WINDOW(quitupgrade_dialog), FALSE);
        gtk_window_set_position(GTK_WINDOW(quitupgrade_dialog),GTK_WIN_POS_MOUSE);
        gtk_window_set_default_size(GTK_WINDOW(quitupgrade_dialog),320,245);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
        gtk_window_set_icon_from_file(GTK_WINDOW(quitupgrade_dialog),image_dir,NULL);
        gtk_widget_set_events(GTK_WINDOW(quitupgrade_dialog),
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

        g_signal_connect(GTK_WINDOW(quitupgrade_dialog), "button_press_event", G_CALLBACK (button_press_event), quitupgrade_dialog);       // 加入事件回调
//        g_signal_connect(GTK_WINDOW(quitupgrade_dialog), "motion_notify_event",G_CALLBACK( motion_notify_event), quitupgrade_dialog);
        g_signal_connect(GTK_WINDOW(quitupgrade_dialog), "button_release_event", G_CALLBACK(button_release_event), quitupgrade_dialog);

        quitupgrade_layout=gtk_layout_new(NULL, NULL);
        gtk_widget_show(quitupgrade_layout);
        gtk_widget_set_size_request(quitupgrade_layout,320,245);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%squitupdate_bg.png"),ANTIVIRUS_IMAGE_DIR);

        quitupdate_bg_image = gtk_image_new_from_file(image_dir);
        gtk_widget_show(quitupdate_bg_image);
        gtk_layout_put(GTK_LAYOUT(quitupgrade_layout),quitupdate_bg_image, 0, 0);

	char tipdialog_css[256];
        snprintf(tipdialog_css,sizeof(tipdialog_css),"%stipdialog.css",ANTIVIRUS_CSS_DIR);

	GFile *file=g_file_new_for_path(tipdialog_css);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);

        quitupdate_dialog_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)quitupdate_dialog_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(quitupdate_dialog_button,17,17);
        gtk_widget_show(quitupdate_dialog_button);
        g_signal_connect(G_OBJECT(quitupdate_dialog_button),"clicked",G_CALLBACK(quitupgrade_warn_dialog),quitupgrade_dialog);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);

        quitupgrade_button_image= gtk_image_new_from_file(image_dir);
        gtk_widget_show(quitupgrade_button_image);
        gtk_button_set_image(quitupdate_dialog_button,quitupgrade_button_image);
	gtk_layout_put(GTK_LAYOUT(quitupgrade_layout),quitupdate_dialog_button,290,-5);
	gtk_widget_set_name(quitupdate_dialog_button,"tipdialog-close");
        gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
	
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%squitupdate_icon.png"),ANTIVIRUS_IMAGE_DIR);

	quitupgrade_icon_image=gtk_image_new_from_file(image_dir);
	gtk_widget_show(quitupgrade_icon_image);
	gtk_layout_put(GTK_LAYOUT(quitupgrade_layout),quitupgrade_icon_image,134,89);
	
	quitupgrade_label=(GtkLabel *)gtk_label_new(str);
	change_font_style((GtkLabel *)quitupgrade_label,"<span foreground=\"#686868\" font_desc='11'>%s</span>");
	gtk_widget_show(quitupgrade_label);
	gtk_layout_put(GTK_LAYOUT(quitupgrade_layout),quitupgrade_label,97,158);

	quitupgrade_OK_button=gtk_button_new();
	gtk_button_set_relief((GtkButton *) quitupgrade_OK_button,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(quitupgrade_OK_button,82,34);
	gtk_widget_show( quitupgrade_OK_button);
	 g_signal_connect(G_OBJECT(quitupgrade_OK_button),"clicked",G_CALLBACK(quitupgrade_select_OK),quitupgrade_dialog);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sdetermine.png"),ANTIVIRUS_IMAGE_DIR);
	quitupgrade_OK_image=gtk_image_new_from_file(image_dir);
	gtk_widget_show(quitupgrade_OK_image);
        gtk_button_set_image(quitupgrade_OK_button,quitupgrade_OK_image);
        gtk_layout_put(GTK_LAYOUT(quitupgrade_layout),quitupgrade_OK_button, 165, 190);
	//gtk_layout_put(GTK_LAYOUT(quitupgrade_layout),quitupgrade_OK_button, 79, 190);
	
	quitupgrade_NO_button=gtk_button_new();
        gtk_button_set_relief((GtkButton *)quitupgrade_NO_button,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(quitupgrade_NO_button,82,34);
        gtk_widget_show(quitupgrade_NO_button);
         g_signal_connect(G_OBJECT(quitupgrade_NO_button),"clicked",G_CALLBACK(quitupgrade_select_NO),quitupgrade_dialog);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%scancel.png"),ANTIVIRUS_IMAGE_DIR);
        quitupgrade_NO_image=gtk_image_new_from_file(image_dir);
        gtk_widget_show(quitupgrade_NO_image);
        gtk_button_set_image(quitupgrade_NO_button,quitupgrade_NO_image);
        gtk_layout_put(GTK_LAYOUT(quitupgrade_layout),quitupgrade_NO_button, 79, 190);

	gtk_container_add(GTK_CONTAINER(quitupgrade_dialog),quitupgrade_layout);	
	gtk_widget_show(quitupgrade_dialog);
	
	return 1;
	
}

void quitupgrade_warn_dialog(GtkWidget *widget,gpointer data)
{
	gtk_widget_destroy(data);
}

void quitupgrade_select_NO(GtkWidget *widget,gpointer data)
{
	gtk_widget_destroy(data);

}
void quitupgrade_select_OK(GtkWidget *widget,gpointer data)
{
	derect_quit_upgrade();
	gtk_widget_destroy(data);
//	int avlStatus = 0;
    /*char upgrade_dir[512];
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
    }*/
//	if((strcmp(gtk_label_get_label(attentionlabel),"<span foreground='#74787c' font_desc='10'>_(正在初始化引擎，请稍候...</span>"))==0){
//		
//	}
//	else{
	int avlStatus = 0;
    char upgrade_dir[512];
           snprintf(upgrade_dir,sizeof(upgrade_dir),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
    avlStatus = get_upgrde_status(upgrade_dir);
    if(avlInit == 0 || avlStatus == 6 ){
	printf("初始化没有完毕\n");

    }else{
	char engin_ver[1024];
//	strncpy(engin_ver,ENGINE_MAIN_PAGE_VER,sizeof(engin_ver));
        cmd_shell_result("dpkg -s cdos-virus | grep Description|cut -d ':' -f2 |sed s/[[:space:]]//g",engin_ver,1024);
        engin_ver[strlen(engin_ver)-1]=0;
        char engin_title[1024];
        snprintf(engin_title,sizeof(engin_title),_("Engine version:%s"),engin_ver);
        //gtk_label_set_text(attentionlabel,engin_title);
        //change_font_style((GtkLabel *)attentionlabel,"<span foreground=\"#74787c\" font_desc='10'>%s</span>");
     //   return 0;
    }
/*	char engin_ver[1024];
        cmd_shell_result("freshclam -V | cut -d ' ' -f2 | cut -d '/' -f-2",engin_ver,1024);
        engin_ver[strlen(engin_ver)-1]=0;
        char engin_title[1024];
        snprintf(engin_title,sizeof(engin_title),_("Engine version:%s"),engin_ver);
        gtk_label_set_text(attentionlabel,engin_title);
        change_font_style((GtkLabel *)attentionlabel,"<span foreground=\"#74787c\" font_desc='10'>%s</span>");*/

}

void derect_quit_upgrade()
{
	 int iupgradeFlag = 0;
    	char szLoginfo[512];
    	char time[30];

        pthread_cancel(avlUpgradeRefreshThreadId);
        pthread_cancel(avlUpgradeRunThreadId);
	char avlUpgradeStatus[256];
        snprintf(avlUpgradeStatus,sizeof(avlUpgradeStatus),"%savlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR);

        iupgradeFlag = get_upgrde_status(avlUpgradeStatus);
	char avlUpgradeLog[256];
	char avlUpgradeDate[256];

	snprintf(avlUpgradeLog,sizeof(avlUpgradeLog),"%savlUpgradeLog.txt",ANTIVIRUS_LOG_DIR);
	snprintf(avlUpgradeDate,sizeof(avlUpgradeDate),"%savlUpgradeDate.txt",ANTIVIRUS_LOG_DIR);
        char day_temp[512];
        char year_temp[512];
        char month_temp[512];
//	char last_update_label[256];
        switch(iupgradeFlag)
        {
                case 1:
            //无需升级
		/*	char avlUpgradeLog[256];
    			snprintf(avlUpgradeLog,sizeof(avlUpgradeLog),"%savlUpgradeLog.txt",ANTIVIRUS_LOG_DIR);
        		if(access(avlUpgradeLog,F_OK)!=0){
        			last_update_label = gtk_label_new(_("update none"));
    			}
    			else{
        			printf("已经有升级文件啦\n");
        			char last_update_time[200];
        			char year[10],month[10],day[10];
        			get_last_upgrade_time(year,month,day);
        			if (get_last_upgrade_time(year,month,day)==0)
        			{
            			last_update_label = gtk_label_new(_("update none"));
        			}
        			else
        			{
        
            				last_update_label = gtk_label_new("");
            				sprintf(last_update_time,_("you last update is:%syear,%smonth,%sday"),year,month,day);
            				gtk_label_set_text(last_update_label,last_update_time);
        			}
    			}
    			change_font_style((GtkLabel *)last_update_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");
    			gtk_widget_show(last_update_label);*/
		//	gtk_widget_hide(last_update_label);
//			gtk_widget_destroy(last_update_label);
			snprintf(szLoginfo,sizeof(szLoginfo),"%s,%s,%s,\n",upgradeBeginTime,_("manual-update"),_("now-is-new"));
		//	if (get_last_upgrade_time(year_temp,month_temp,day_temp)==0)
                  //      {
                                write_upgrade_log(avlUpgradeDate,szLoginfo);
                    //    }

                        write_upgrade_log(avlUpgradeLog,szLoginfo);
/*			char avlUpgradeLog1[256];
                        snprintf(avlUpgradeLog1,sizeof(avlUpgradeLog1),"%savlUpgradeLog.txt",ANTIVIRUS_LOG_DIR);
					char last_update_time[200];
                                	char year[10],month[10],day[10];
                                	get_last_upgrade_time(year,month,day);
                                        last_update_label = gtk_label_new("");
                                        sprintf(last_update_time,_("you last update is:%syear,%smonth,%sday"),year,month,day);
                                        gtk_label_set_text(last_update_label,last_update_time);
					gtk_widget_show(last_update_label);
                                //}
                       // }
                        change_font_style((GtkLabel *)last_update_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");
			printf("开始显示last—_updatelable\n");
                        gtk_widget_show(last_update_label);*/
                        break;
                case 2:
			snprintf(szLoginfo,sizeof(szLoginfo),"%s,%s,%s,\n",upgradeBeginTime,_("manual-update"),_("upgrade success"));
                        write_upgrade_log(avlUpgradeLog,szLoginfo);
			char avlUpgradeLog2[256];
                        snprintf(avlUpgradeLog2,sizeof(avlUpgradeLog2),"%savlUpgradeLog.txt",ANTIVIRUS_LOG_DIR);
                        if(access(avlUpgradeLog2,F_OK)!=0){
                                //last_update_label = gtk_label_new(_("update none"));
                        }
                        else{
                                printf("已经有升级文件啦\n");
                                char last_update_time[200];
                                char year[10],month[10],day[10];
                                get_last_upgrade_time(year,month,day);
                                if (get_last_upgrade_time(year,month,day)==0)
                                {
                                //last_update_label = gtk_label_new(_("update none"));
                                }
                                else
                                {

                                  //      last_update_label = gtk_label_new("");
                                    //    sprintf(last_update_time,_("you last update is:%syear,%smonth,%sday"),year,month,day);
                                      //  gtk_label_set_text(last_update_label,last_update_time);
                                }
                        }
                        //change_font_style((GtkLabel *)last_update_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");
                        //gtk_widget_show(last_update_label);
                        break;
                case 3:
                        snprintf(szLoginfo,sizeof(szLoginfo),"%s,%s,%s,\n",upgradeBeginTime,_("manual-update"),_("network problems,upgrade fail"));
                        write_upgrade_log(avlUpgradeLog,szLoginfo);
                        break;
                case 4:
                        snprintf(szLoginfo,sizeof(szLoginfo),"%s,%s,%s,\n",upgradeBeginTime,_("manual-update"),_("engine is in use ,upgrade fail"));
                        write_upgrade_log(avlUpgradeLog,szLoginfo);

        }
	
        gtk_widget_destroy(dialog);
	//gtk_widget_show(last_update_label);
}

//退出升级函数
gboolean quitupgrade(GtkWidget *widget,gpointer data)
{
    int iupgradeFlag = 0;
    char szLoginfo[512];
    char time[30];
    int iUpgradeStatus = 0;
    gint isok = 0;
    char avlUpgradeStatus[256];
        snprintf(avlUpgradeStatus,sizeof(avlUpgradeStatus),"%savlUpgradeStatus.txt",ANTIVIRUS_TEMP_DIR);

    iUpgradeStatus = get_upgrde_status(avlUpgradeStatus);
    if(iUpgradeStatus == 1 || iUpgradeStatus ==2 || iUpgradeStatus==3 || iUpgradeStatus ==4|| iUpgradeStatus ==22)
    {
        isok = 0;
    }
    else
    {
        isok=quitupgrade_dialog_create(_("are you sure to quit update?"));
	
    }
    if(isok==0)
    {
	derect_quit_upgrade();
        return FALSE;
     }
     if(isok==1)
     	return TRUE;

	
}

