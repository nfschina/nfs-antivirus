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
#include <gdk/gdk.h>
#include "about-page.h"
#include "scanprocess-page.h"
#include "upgrade-page.h"
#include "home-page.h"
#include "log-page.h"
#include "setting-page.h"
#include "../scan.h"
#include "../deal/tray_handle.h"
#include "../deal/home_handle.h"
#include "../page/setting-page.h"
#include "../real_protect_result_page.h"
#include "../function.h"
extern pthread_t  showThreadId;
GtkWidget *vbox,*thingbox;
GtkWidget *thingbox,*vbox1,*vboxlog,*hboxlog,*logfix1;
GtkWidget *last_scan_button,*hide_button,*time_layout;
GtkWidget *title_layout,*versionimage;
extern GtkWidget *layout_main;//主界面菜单栏
extern GtkWidget *devide_content_box;
extern GtkWidget *detail_result_table;
extern GtkWidget *common_settings_fix,*common_setting_content_box;
extern GtkWidget *scan_style_setting_fix,*scan_style_setting_content_box;
extern GtkWidget *anti_scan_settings_fix,*anti_scan_setting_content_box;
extern GtkWidget *update_settings_fix,*update_setting_content_box;
int start_or_close_defence;
GtkWidget * mainPageIntroduceImage; //主界面标题图片
GtkWidget *click_start_defence_button,*click_start_defence_image,*click_close_defence_button,*click_close_defence_image;//主界面防御按钮
GtkWidget *allscan_button_image,*quickscan_button_image,*selfscan_button_image,*antiallscanbutton,*antiquickbutton,*antimyselfbutton;
GtkWidget *feedback_label_button_white_bg,*feedback_label_button,*check_update_button,*check_update_button_white_bg;
GtkWidget *antibutton,*setbutton,*logbutton,*aboutbutton;
GtkWidget *quickscan_above_image,*allscan_above_image,*selfscan_above_image;
GtkWidget *look_devide_label_button,*look_devide_label;
GtkWidget *start_defence_image=NULL;
GtkWidget *table1,*table2,*table3,*table4,*table6,*table7,*auto_scan_usb_opt,*real_protect_result_table;
GtkStatusIcon *trayIcon;
GtkWidget *start_image,*close_image;
GtkWidget *last_scan_label;
GtkWidget *versionlayout;
GtkWidget * look_last_update_button;
char last_scan_time[200];
int area = 1; //当前区域标志
int isupdting = 0 ; //当前是否正在升级标志
extern void antivirus_settings_page(GtkWidget *table);

gboolean drag = FALSE;   // 只在左键按下时拖动窗体
int nX = 0;
int nY = 0;
int start_defence_show = 1;
int last_update_show = 1;
int look_devide_show = 1;
int last_scan_time_show = 1;
GtkLabel * attentionlabel; //显示版本
GtkWindow *window;//主窗口指针
char g_current_user[256]; //当前启动杀毒软件用户
void sleep_func(GtkWidget *widget,int flag);

/*******全盘扫描函数***********/
void all_scan_type(GtkWidget *widget,gpointer data)
{
     //判断引擎是否初始化完成
    if(checkEngineStatus(widget, data))
                return;

    //执行扫描函数
    execution_scan(ALL_SCAN,NULL);
}

/****快速扫描函数*****/
void quick_scan_type(GtkWidget *widget,gpointer data)
{
     //判断引擎是否初始化完成
    if(checkEngineStatus(widget, data))
        return;

    //执行扫描函数
    execution_scan(FAST_SCAN,NULL);

}

/**********自定义扫描************/
void custom_scan_type(GtkWidget *widget,gpointer data)
{
 //判断引擎是否初始化完成
    if(checkEngineStatus(widget, data))
                return;

    //执行扫描函数
    execution_scan(CUSTOM_SCAN,NULL);
}

/**扫描指定文件*/
void specified_scan_type(char *scanPath,int scanType)
{
    //判断引擎是否初始化完成
    execution_scan(scanType,scanPath);
}

/***获取主动防御是否全部开启状态*/
int get_defence_status()
{
        int boot_start_select,autocheck_mobiledev_select,autocheck_download_select,protect_status;
        
		char set_file_path[1024];
		snprintf(set_file_path,sizeof(set_file_path),"%s%s_setting.txt",ANTIVIRUS_CONF_DIR,g_current_user);
        boot_start_select=get_antivirus_setting_item_inttype("boot-start",set_file_path);
        autocheck_mobiledev_select=get_antivirus_setting_item_inttype("auto-check-mobile-dev",set_file_path);
        autocheck_download_select=get_antivirus_setting_item_inttype("auto-check-download",set_file_path);
	protect_status = get_antivirus_setting_item_inttype("check-real-time-protect",set_file_path);
        if (boot_start_select==1 && autocheck_mobiledev_select ==1 && autocheck_download_select ==1 && protect_status==1)
        {
                return 1;       //主动防御已打开
        }
        else
        {
                return 0;       //主动防御没有全部打开或全部关闭
        }

}

//显示杀毒软件版本号函数
void show_antivirus_version()
{
        GtkWidget *antibutton;
	char image_dir[256];
	char version_css[100];	
	
        GtkWidget *versionbox,*feedback_label,*check_update_label,*feedback_label_white_bg;
        versionlayout = gtk_layout_new(NULL, NULL);
        gtk_widget_show(versionlayout);
        gtk_widget_set_size_request(versionlayout, 800, 25);

        snprintf(image_dir,sizeof(image_dir),_("%sversion.png"),ANTIVIRUS_IMAGE_DIR);
        versionimage = gtk_image_new_from_file(image_dir);
        gtk_widget_show(versionimage);
        gtk_layout_put(GTK_LAYOUT(versionlayout), versionimage, 0, 0);
	
        snprintf(version_css,sizeof(version_css),_("%sversion.css"),ANTIVIRUS_CSS_DIR);
	GFile *file=g_file_new_for_path(version_css);
        GdkScreen *screen;
        screen = gdk_screen_get_default();
        GtkCssProvider *cssProvider;
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(cssProvider, file, NULL);
 
        attentionlabel = (GtkLabel *)gtk_label_new(_("Engine Initing..."));
        gchar all_totleattentionlabel[100];
        change_font_style((GtkLabel *)attentionlabel,"<span foreground=\"#74787c\" font_desc='10'>%s</span> ");
	gtk_widget_show(attentionlabel);
	gtk_layout_put(GTK_LAYOUT(versionlayout),attentionlabel, 650, 0);

/*	feedback_label = gtk_label_new(_("feedback"));
        change_font_style((GtkLabel *)feedback_label,"<span foreground=\"#74787c\" font_desc='10' underline='single'>%s</span>");
	gtk_widget_show(feedback_label);*/
	feedback_label_button = gtk_button_new();
	gtk_button_set_relief((GtkButton *)feedback_label_button,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(feedback_label_button,36,19);
//	gtk_container_add(GTK_CONTAINER(feedback_label_button),feedback_label);
	gtk_widget_show(feedback_label_button);
        gtk_widget_set_name(feedback_label_button,"feedback");
	g_signal_connect(G_OBJECT(feedback_label_button),"clicked",G_CALLBACK(feedback_func),NULL);	
//	g_signal_connect(G_OBJECT(feedback_label_button),"enter",G_CALLBACK(enter_feedback),feedback_label);

	char feedback_image_path[1024];
	memset(feedback_image_path,0,sizeof(feedback_image_path));
	snprintf(feedback_image_path,sizeof(feedback_image_path),_("%sfeedback_white_bg.png"),ANTIVIRUS_IMAGE_DIR);
	feedback_label_white_bg = gtk_image_new_from_file (feedback_image_path);	
        gtk_widget_show(feedback_label_white_bg);
        feedback_label_button_white_bg = gtk_button_new();
	gtk_button_set_image(feedback_label_button_white_bg,feedback_label_white_bg);
        gtk_button_set_relief((GtkButton *)feedback_label_button_white_bg,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(feedback_label_button_white_bg,36,16);
	gtk_widget_hide(feedback_label_button_white_bg);
        g_signal_connect(G_OBJECT(feedback_label_button_white_bg),"clicked",G_CALLBACK(feedback_func),NULL);
        g_signal_connect(G_OBJECT(feedback_label_button_white_bg),"enter",G_CALLBACK(enter_feedback_white_bg),feedback_label);
        gtk_layout_put(GTK_LAYOUT(versionlayout), feedback_label_button_white_bg, 20, 0);
	gtk_layout_put(GTK_LAYOUT(versionlayout), feedback_label_button, 20, -5);

/*	check_update_label = gtk_label_new(_("checkupdate"));
	change_font_style((GtkLabel *)check_update_label,"<span foreground=\"#59c1f7\" font_desc='10' underline='single'>%s</span>");
	gtk_widget_show(check_update_label);*/
	check_update_button = gtk_button_new();
	gtk_button_set_relief((GtkButton *)check_update_button,GTK_RELIEF_NONE);
	gtk_widget_set_size_request(check_update_button,62,19);
//	gtk_container_add(GTK_CONTAINER(check_update_button),check_update_label);
	gtk_widget_show(check_update_button);
	gtk_widget_set_name(check_update_button,"check_update");
	
	char checkupdate_image_path[1024];
        memset(checkupdate_image_path,0,sizeof(checkupdate_image_path));
        snprintf(checkupdate_image_path,sizeof(checkupdate_image_path),_("%supdate_white_bg.png"),ANTIVIRUS_IMAGE_DIR);
	GtkWidget *check_update_label_white_bg = gtk_image_new_from_file (checkupdate_image_path);	
       // gtk_widget_show(check_update_label_white_bg);
        check_update_button_white_bg = gtk_button_new();
//	gtk_button_set_image(check_update_button_white_bg,check_update_label_white_bg);
        gtk_button_set_relief((GtkButton *)check_update_button_white_bg,GTK_RELIEF_NONE);
        gtk_widget_set_size_request(check_update_button_white_bg,62,19);
	g_signal_connect(G_OBJECT(check_update_button_white_bg),"clicked",G_CALLBACK(check_update),NULL);
  //      g_signal_connect(G_OBJECT(check_update_button_white_bg),"enter",G_CALLBACK(enter_checkupdate_white_bg),check_update_label);
//	gtk_layout_put(GTK_LAYOUT(versionlayout), check_update_button_white_bg, 142, 0);

	gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
	gtk_layout_put(GTK_LAYOUT(versionlayout), check_update_button, 142, -4);
 	g_signal_connect(G_OBJECT(check_update_button),"clicked",G_CALLBACK(check_update),NULL);
//	g_signal_connect(G_OBJECT(check_update_button),"enter",G_CALLBACK(enter_checkupdate),check_update_label);

        gtk_container_add(GTK_CONTAINER(vbox),versionlayout);
        gtk_widget_show(vbox);
}

//询问是否退出杀毒软件函数
gint  closeall(GtkWidget *widget,gpointer data)
{
      show_warn_info(_("sure-to-quit"),2);
}
void min_window(GtkWidget *widget,gpointer data){
	gtk_window_iconify(data);
}
//点击杀毒按钮时，显示杀毒按钮界面，隐藏其它界面
void antiwindow(GtkWidget *widget,gpointer data)
{
	char image_dir[256];
	if (gtk_widget_get_visible (start_defence_image))
	{
		gtk_widget_hide(start_defence_image);	
	}
	
	if (click_close_defence_button !=NULL)
        {
                gtk_widget_hide(click_close_defence_button);
        }
        if (click_start_defence_button!=NULL)
        {
                gtk_widget_hide(click_start_defence_button);
        }
        if (get_defence_status()==1)
        {
                gtk_widget_hide(click_start_defence_button);
                gtk_widget_show(click_close_defence_button);
        }
        else
        {
		gtk_widget_hide(click_close_defence_button);
                gtk_widget_show(click_start_defence_button);
        }
	gtk_widget_hide(start_defence_image);
        if(area==1)
        {
                return;
        }
        if(area==2)
        {
		gtk_button_set_relief((GtkButton *)setbutton,GTK_RELIEF_NONE);
                snprintf(image_dir,sizeof(image_dir),_("%ssetting.png"),ANTIVIRUS_IMAGE_DIR);
                GtkWidget *leave_setbutton_image = gtk_image_new_from_file(image_dir);

//                gtk_button_set_image(setbutton,leave_setbutton_image);

                gtk_widget_hide(table2);
		if(box!=NULL)
	            gtk_widget_destroy(box);
    		white_list_settings(while_list_content_box);
		free_devide_file_list_memory();
                free_white_list_file_memory();
        }
        if(area==3)
        {
		gtk_button_set_relief((GtkButton *)logbutton,GTK_RELIEF_NONE);
		memset(image_dir,0,sizeof(image_dir));
                snprintf(image_dir,sizeof(image_dir),_("%slogging.png"),ANTIVIRUS_IMAGE_DIR);
                GtkWidget *leave_logbutton_image = gtk_image_new_from_file(image_dir);

  //              gtk_button_set_image(logbutton,leave_logbutton_image);

                gtk_widget_hide(table3);
                free_log_list_memory();
        }
        if(area==4)
        {
		gtk_button_set_relief((GtkButton *)aboutbutton,GTK_RELIEF_NONE);
		memset(image_dir,0,sizeof(image_dir));
                snprintf(image_dir,sizeof(image_dir),_("%sabout.png"),ANTIVIRUS_IMAGE_DIR);
                GtkWidget *leave_aboutbutton_image = gtk_image_new_from_file(image_dir);

    //            gtk_button_set_image(aboutbutton,leave_aboutbutton_image);
                gtk_widget_hide(table4);
        }
        if(area==5)
        {
        }
        gtk_widget_show(table1);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%santi_introduce_title.gif"),ANTIVIRUS_IMAGE_DIR);
	gtk_image_set_from_file (mainPageIntroduceImage,image_dir);
	gtk_button_set_relief((GtkButton *)widget,GTK_RELIEF_NONE);

	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%santi_enter.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_homepage_antibutton_image = gtk_image_new_from_file(image_dir);

      //  gtk_button_set_image(widget,enter_homepage_antibutton_image);
        area = 1;
}
//点击设置按钮，显示设置界面，隐藏其它界面
void setwindow(GtkWidget *widget,gpointer data)
{
	char image_dir[256];
        if(area==1)
        {
		gtk_button_set_relief((GtkButton *)antibutton,GTK_RELIEF_NONE);
	        snprintf(image_dir,sizeof(image_dir),_("%santi.png"),ANTIVIRUS_IMAGE_DIR);
                GtkWidget *leave_antibutton_image = gtk_image_new_from_file(image_dir);

        //        gtk_button_set_image(antibutton,leave_antibutton_image);
                gtk_widget_hide(table1);
        }
        if(area==2)
        {
                return;
        }
        if(area==3)
        {
		gtk_button_set_relief((GtkButton *)logbutton,GTK_RELIEF_NONE);
		memset(image_dir,0,sizeof(image_dir));
        	snprintf(image_dir,sizeof(image_dir),_("%slogging.png"),ANTIVIRUS_IMAGE_DIR);

                GtkWidget *leave_logbutton_image = gtk_image_new_from_file(image_dir);

          //      gtk_button_set_image(logbutton,leave_logbutton_image);

                gtk_widget_hide(table3);
                free_log_list_memory();
        }
        if(area==4)
        {
		gtk_button_set_relief((GtkButton *)aboutbutton,GTK_RELIEF_NONE);
		memset(image_dir,0,sizeof(image_dir));
                snprintf(image_dir,sizeof(image_dir),_("%sabout.png"),ANTIVIRUS_IMAGE_DIR);

                GtkWidget *leave_aboutbutton_image = gtk_image_new_from_file(image_dir);
                
            //    gtk_button_set_image(aboutbutton,leave_aboutbutton_image);
                gtk_widget_hide(table4);
        }
        if(area==5)
        {
        }
	gtk_button_set_relief((GtkButton *)setbutton,GTK_RELIEF_NONE);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sset_enter.png"),ANTIVIRUS_IMAGE_DIR);
        GtkWidget *enter_set_image = gtk_image_new_from_file(image_dir);

       // gtk_button_set_image(setbutton,enter_set_image);

        gtk_widget_show(table2);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%ssetting_introduce_title.png"),ANTIVIRUS_IMAGE_DIR);
	gtk_image_set_from_file (mainPageIntroduceImage,image_dir);
	gtk_notebook_set_current_page((GtkNotebook *)notebook,0);
	if(devide_file_box!=NULL)
		gtk_widget_destroy(devide_file_box);
	GtkWidget *content_box;
        look_devide_file_settings(devide_content_box);
	if(common_settings_fix!=NULL)
		gtk_widget_destroy(common_settings_fix);
	common_settings(common_setting_content_box);


	if(anti_scan_settings_fix!=NULL)
		gtk_widget_destroy(anti_scan_settings_fix);
        anti_scan_settings(anti_scan_setting_content_box);

	if(update_settings_fix!=NULL)
        	gtk_widget_destroy(update_settings_fix);
        update_settings(update_setting_content_box);
	
        area = 2;
}



//设置杀毒软件图标展示在系统托盘中
void set_icon_in_tray(char *info)
{
    char image_dir[256];
    //设置托盘图标图片
    snprintf(image_dir,sizeof(image_dir),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
    trayIcon  = gtk_status_icon_new_from_file (image_dir);

    //为图片创建点击菜单栏
    GtkWidget *menu, *menuItemView, *menuItemExit;
    menu = gtk_menu_new();
    menuItemView = gtk_menu_item_new_with_label (_("ViewAntivirus"));
    menuItemExit = gtk_menu_item_new_with_label (_("ExitAntivirus"));
    g_signal_connect (G_OBJECT (menuItemView), "activate", G_CALLBACK (trayView), window);
    g_signal_connect (G_OBJECT (menuItemExit), "activate", G_CALLBACK (destroy), NULL);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItemView);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItemExit);
    gtk_widget_show_all (menu);

    g_signal_connect(GTK_STATUS_ICON (trayIcon), "activate", G_CALLBACK(trayIconActivated),window);
    g_signal_connect(GTK_STATUS_ICON (trayIcon), "popup-menu",G_CALLBACK(trayIconPopup), menu);

    GtkWidget *menuBar, *menuItemTopLvl, *mainMenu, *mainMenuItemExit;
    menuBar = gtk_menu_bar_new ();
    menuItemTopLvl = gtk_menu_item_new_with_label ("Menu");
    gtk_menu_shell_append (GTK_MENU_SHELL (menuBar), menuItemTopLvl);

    mainMenu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuItemTopLvl), mainMenu);
    mainMenuItemExit = gtk_menu_item_new_with_label ("Quit");
    g_signal_connect (G_OBJECT (mainMenuItemExit), "activate", G_CALLBACK (destroy), NULL);
    gtk_menu_shell_append (GTK_MENU_SHELL (mainMenu), mainMenuItemExit);

    g_signal_connect (G_OBJECT (window), "window-state-event", G_CALLBACK (window_state_event), trayIcon);

    gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
    gtk_status_icon_set_has_tooltip (trayIcon,TRUE);
    gtk_status_icon_set_tooltip_text (trayIcon,_("anti-virus"));

}
//鼠标点击窗口事件
gint button_press_event(GtkWidget * widget,GdkEventButton * event, gpointer data)
{
     /*   if (event->button == 1)       // 判断是否左键按下
        {
                 drag = TRUE;
                 nX = event->x;  // 取得鼠标相对于窗口的位置
                 nY = event->y;
        }
        return TRUE;*/

if(event->type=GDK_BUTTON_PRESS)
                         {
                  if (event->button == 1)       // 判断是否左键按>下
                 {  gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),event->button,event->x_root,event->y_root,event->time);

                return FALSE;
     }
}}


gint button_release_event(GtkWidget * widget, GdkEventButton * event,gpointer data)         // 鼠标抬起事件
{
        if (event->button == 1)
                 drag = FALSE;
        return TRUE;
}
gint motion_notify_event(GtkWidget * widget, GdkEventButton * event,gpointer data)         // 鼠标移动事件
{
        if (drag)
        {
                 int x, y;
                 GtkWidget *win = (GtkWidget *) data;
                 gtk_window_get_position((GtkWindow *) win, &x, &y);         // 取窗体绝对坐标
                 gtk_window_move((GtkWindow *) win, x + event->x - nX,
                             y + event->y - nY);// 移动窗体
        }
        return TRUE;
}

//创建杀毒软件页面框架
void create_frame_page()
{
    GtkWidget *antibuttonimage,*setbuttonimage,*logbuttonimage,*updatetopbutton,*updatebuttonimage,*aboutbuttonimage,*title_label,*title_box,*title_min_image,*title_min_button,*title_close_image,*title_close_button;
    char image_dir[256] ; 
    window = (GtkWindow *)gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(window,_("antivirus soft"));
    gtk_window_set_wmclass(window,"cdos-antivirus","cdos-antivirus");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),800,550);
    gtk_window_set_resizable(window, 0);

    snprintf(image_dir,sizeof(image_dir),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
    gtk_window_set_icon_from_file(GTK_WINDOW(window),image_dir,NULL);
    
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);   // 去掉边框
    gtk_widget_set_events(window,  // 设置窗体获取鼠标事件
                   GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK
                   | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

    g_signal_connect(GTK_WINDOW(window), "button_press_event", G_CALLBACK (button_press_event), window);       // 加入事件回调
//    g_signal_connect(GTK_WINDOW(window), "motion_notify_event",G_CALLBACK( motion_notify_event), window);
    g_signal_connect(GTK_WINDOW(window), "button_release_event", G_CALLBACK(button_release_event), window);  

    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(destroy),NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window),vbox);
    title_layout = gtk_layout_new(NULL, NULL);
    gtk_widget_show(title_layout);
    gtk_widget_set_size_request(title_layout, 800, 20);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%stitle_top.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *title_top_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(title_top_image);
    gtk_layout_put(GTK_LAYOUT(title_layout),title_top_image, 0, 0);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%stitle_top_add.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *title_top_add_iamge = gtk_image_new_from_file(image_dir);
    gtk_widget_show(title_top_add_iamge);
    gtk_layout_put(GTK_LAYOUT(title_layout),title_top_add_iamge, 0, 0);

    char anti_ver[1024];
    cmd_shell_result("dpkg -s cdos-antivirus | grep Version | cut -d ':' -f2",anti_ver,1024);
    anti_ver[strlen(anti_ver)-1]=0;
    char anti_title[1024];
    //snprintf(anti_title,sizeof(anti_title),_("nfs antivirus soft V%s"),anti_ver);
    strcpy(anti_title,_("nfs antivirus soft"));
    //replace_string(anti_title,"-",".");
    title_label = gtk_label_new(anti_title);
    change_font_style((GtkLabel *)title_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span>");
    gtk_widget_show(title_label);
    
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%smini.png"),ANTIVIRUS_IMAGE_DIR);
    title_min_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(title_min_image);

    char home_title_css[100];
    snprintf(home_title_css,sizeof(home_title_css),_("%shome-title.css"),ANTIVIRUS_CSS_DIR);

    GFile *file=g_file_new_for_path(home_title_css);
    GdkScreen *screen;
    screen = gdk_screen_get_default();
    GtkCssProvider *cssProvider;
    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(cssProvider, file, NULL);


    title_min_button = gtk_button_new();
    gtk_container_set_border_width(title_min_button,0);
    gtk_widget_grab_default(title_min_button);
    gtk_button_set_relief((GtkButton *)title_min_button,GTK_RELIEF_NONE);
 //   gtk_button_set_image(title_min_button,title_min_image);
    gtk_widget_set_name(title_min_button,"home-mini");
    gtk_widget_set_size_request(title_min_button,15,15);
    gtk_widget_show(title_min_button);
    g_signal_connect(G_OBJECT(title_min_button),"clicked",G_CALLBACK(min_window),window);
   // g_signal_connect(G_OBJECT(title_min_button),"enter",G_CALLBACK(enter_min_button),window);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sclose.png"),ANTIVIRUS_IMAGE_DIR);
    title_close_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(title_close_image);
    title_close_button = gtk_button_new();
   
    gtk_button_set_relief((GtkButton *)title_close_button,GTK_RELIEF_NONE);
    gtk_widget_set_size_request(title_close_button,15,15);
//    gtk_button_set_image(title_close_button,title_close_image);
    gtk_widget_set_name(title_close_button,"home-close");
    gtk_widget_show(title_close_button);
  //  g_signal_connect(G_OBJECT(title_close_button),"enter",G_CALLBACK(enter_close_button),window);
    gtk_layout_put(GTK_LAYOUT(title_layout),title_close_button, 770, -5);
    gtk_layout_put(GTK_LAYOUT(title_layout),title_min_button, 745, -8);
    gtk_layout_put(GTK_LAYOUT(title_layout),title_label, 10, 4);


    layout_main = gtk_layout_new(NULL, NULL);
    gtk_widget_show(layout_main);
    gtk_widget_set_size_request(layout_main, 800, 117);
    
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%smain_title_background.png"),ANTIVIRUS_IMAGE_DIR); 
    GtkWidget *title_back_iamge  = gtk_image_new_from_file(image_dir);
    gtk_widget_show(title_back_iamge);
    gtk_layout_put(GTK_LAYOUT(layout_main),title_back_iamge, 0, 0);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%stitle-bottom.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *title_bottom_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(title_bottom_image);
    gtk_layout_put(GTK_LAYOUT(layout_main),title_bottom_image, 0, 0);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%santi_introduce_title.gif"),ANTIVIRUS_IMAGE_DIR);
    mainPageIntroduceImage = gtk_image_new_from_file(image_dir);
    gtk_widget_show(mainPageIntroduceImage);
    gtk_layout_put(GTK_LAYOUT(layout_main), mainPageIntroduceImage, 20, 50);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%smenu_space.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *menu_space_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(menu_space_image);
    gtk_layout_put(GTK_LAYOUT(layout_main), menu_space_image, 454, 0);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%santi_enter.png"),ANTIVIRUS_IMAGE_DIR);
    antibuttonimage = gtk_image_new_from_file(image_dir);
    gtk_widget_show(antibuttonimage);
    antibutton = gtk_button_new();
    gtk_button_set_relief((GtkButton *)antibutton,GTK_RELIEF_NONE);
   // gtk_button_set_image(antibutton,antibuttonimage);
    gtk_widget_set_size_request(antibutton, 80, 105);
    gtk_widget_set_name(antibutton,"main-entry");
    gtk_layout_put(GTK_LAYOUT(layout_main), antibutton, 458, 3);
    gtk_widget_show(antibutton);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%smenu_space.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *menu_space_image3 = gtk_image_new_from_file(image_dir);
    gtk_widget_show(menu_space_image3);
    gtk_layout_put(GTK_LAYOUT(layout_main), menu_space_image3, 542, 0);

    setbutton = gtk_button_new();
    gtk_button_set_relief((GtkButton *)setbutton,GTK_RELIEF_NONE);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%ssetting.png"),ANTIVIRUS_IMAGE_DIR);
    setbuttonimage = gtk_image_new_from_file(image_dir);
    gtk_widget_show(setbuttonimage);
   // gtk_button_set_image(setbutton, setbuttonimage);
    gtk_widget_show(setbutton);
    gtk_widget_set_size_request(setbutton, 80, 105);
    gtk_widget_set_name(setbutton,"home-set-button");
    gtk_layout_put(GTK_LAYOUT(layout_main), setbutton, 545, 3);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%smenu_space.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *menu_space_image2 = gtk_image_new_from_file(image_dir);
    gtk_widget_show(menu_space_image2);
    gtk_layout_put(GTK_LAYOUT(layout_main),menu_space_image2, 627, 0);

    logbutton = gtk_button_new();
    gtk_button_set_relief((GtkButton *)logbutton,GTK_RELIEF_NONE);
    gtk_widget_set_size_request(logbutton, 82, 105);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%slogging.png"),ANTIVIRUS_IMAGE_DIR);
    logbuttonimage = gtk_image_new_from_file(image_dir);
    gtk_widget_show(logbuttonimage);
   // gtk_button_set_image(logbutton, logbuttonimage);
    gtk_widget_set_name(logbutton,"home-log-button");
    gtk_widget_show(logbutton);
    gtk_layout_put(GTK_LAYOUT(layout_main), logbutton, 630, 3);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%smenu_space.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *menu_space_image1 = gtk_image_new_from_file(image_dir);
    gtk_widget_show(menu_space_image1);
    gtk_layout_put(GTK_LAYOUT(layout_main),menu_space_image1, 715, 0);

    aboutbutton = gtk_button_new();
    gtk_button_set_relief((GtkButton *)aboutbutton,GTK_RELIEF_NONE);
    gtk_widget_set_size_request(aboutbutton, 80, 105);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sabout.png"),ANTIVIRUS_IMAGE_DIR);
    aboutbuttonimage = gtk_image_new_from_file(image_dir);
    gtk_widget_show(aboutbuttonimage);
   // gtk_button_set_image(aboutbutton,aboutbuttonimage);
    gtk_widget_set_name(aboutbutton,"home-about-button");
    gtk_widget_show(aboutbutton);
    gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
    gtk_layout_put(GTK_LAYOUT(layout_main), aboutbutton, 718, 3);


    gtk_container_add(GTK_CONTAINER(vbox),title_layout);
    gtk_container_add(GTK_CONTAINER(vbox),layout_main);
    
    g_signal_connect(G_OBJECT(antibutton),"clicked",G_CALLBACK(antiwindow),NULL);
    g_signal_connect(G_OBJECT(setbutton),"clicked",G_CALLBACK(setwindow),NULL);
    g_signal_connect(G_OBJECT(logbutton),"clicked",G_CALLBACK(logwindow),NULL);
    g_signal_connect(G_OBJECT(aboutbutton),"clicked",G_CALLBACK(aboutwindow),NULL);
    g_signal_connect(G_OBJECT(title_close_button),"clicked",G_CALLBACK(closeall),window);
    g_signal_connect(G_OBJECT(title_min_button),"clicked",G_CALLBACK(min_window),window);

}
/**自定义扫描显示选择目录界面函数*/
void myself_scan_type(GtkWidget *widget,gpointer data)
{

        int retScanStatus = -1;
	char image_dir[256];
        if(checkEngineStatus(widget,data))
                return;
        //检查当前是否正在扫描
        retScanStatus = checkScanStatus(CUSTOM_SCAN);
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

        if(isupdting==1){
                GtkWidget *dialog,*fix,*updatinglabel,*vbox;
                fix = gtk_fixed_new();
                dialog = gtk_dialog_new();
                gtk_window_set_default_size(GTK_WINDOW(dialog),200,100);
                gtk_window_set_title(GTK_WINDOW(dialog), _("updating"));
                snprintf(image_dir,sizeof(image_dir),_("%sNFSantivirus.png"),ANTIVIRUS_IMAGE_DIR);
                gtk_window_set_icon_from_file(GTK_WINDOW(dialog),image_dir,NULL);
                vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
                gtk_container_add(GTK_CONTAINER(vbox),fix);
                updatinglabel = gtk_label_new(_("scaning-please-wait"));
                gtk_fixed_put((GtkFixed *)fix,updatinglabel,50,20);
                gtk_box_pack_start(GTK_BOX (gtk_dialog_get_content_area((GtkDialog *)dialog)),vbox,FALSE,FALSE,10);
                gtk_widget_show_all(dialog);
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
        }
        else
	{
	char alvstatus[256];
        snprintf(alvstatus,sizeof(alvstatus),"%savlstatus.txt",ANTIVIRUS_TEMP_DIR);
         write_antivirus_status(alvstatus,"3");
        filechooser((GtkWidget *)window);
	}
}

void sleep_func(GtkWidget *widget,int flag)
{
	if (flag==0)
	{
		usleep(3000);
	}
}
//创建扫描按钮
void create_scan_button()
{
    GtkWidget *layout,*antibutton,*antivbox,*anti_layout;
    char image_dir[256];
    thingbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(thingbox, 800, 400);
    gtk_widget_show(thingbox);
    table1 = gtk_grid_new();
    antivbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_show(antivbox);
    anti_layout = gtk_layout_new(NULL, NULL);
    gtk_widget_set_size_request(anti_layout, 800, 400);
    gtk_widget_show(anti_layout);
    
    snprintf(image_dir,sizeof(image_dir),_("%santi_bg.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *anti_bg_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(anti_bg_image);
    gtk_layout_put(GTK_LAYOUT(anti_layout), anti_bg_image, 0, 0);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sallscan_image.png"),ANTIVIRUS_IMAGE_DIR);
    allscan_above_image=gtk_image_new_from_file(image_dir);
    gtk_widget_show(allscan_above_image);
    gtk_layout_put(GTK_LAYOUT(anti_layout), allscan_above_image, 146, 65);

    char home_anti_css[256];
    snprintf(home_anti_css,sizeof(home_anti_css),_("%shome-anti.css"),ANTIVIRUS_CSS_DIR);
    GFile *file=g_file_new_for_path(home_anti_css);
    GdkScreen *screen;
    screen = gdk_screen_get_default();
    GtkCssProvider *cssProvider;
    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(cssProvider, file, NULL);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sallscan.png"),ANTIVIRUS_IMAGE_DIR);
    allscan_button_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(allscan_button_image);
    antiallscanbutton = gtk_button_new();
    gtk_button_set_relief((GtkButton *)antiallscanbutton,GTK_RELIEF_NONE);
    gtk_widget_show(antiallscanbutton);
    gtk_widget_set_size_request(antiallscanbutton, 138, 45);
    gtk_widget_show(antiallscanbutton);
   // gtk_button_set_image(antiallscanbutton, allscan_button_image);
    gtk_widget_set_name(antiallscanbutton,"home-allscan");
    gtk_layout_put(GTK_LAYOUT(anti_layout), antiallscanbutton, 135, 185);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%squickscan_image.png"),ANTIVIRUS_IMAGE_DIR);
    quickscan_above_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(quickscan_above_image);
    gtk_layout_put(GTK_LAYOUT(anti_layout), quickscan_above_image, 340, 65);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%squicksacn.png"),ANTIVIRUS_IMAGE_DIR);
    quickscan_button_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(quickscan_button_image);
    antiquickbutton = gtk_button_new();
    gtk_button_set_relief((GtkButton *)antiquickbutton,GTK_RELIEF_NONE);
    gtk_widget_show(antiquickbutton);
    gtk_widget_set_size_request(antiquickbutton, 138, 45);
   // gtk_button_set_image(antiquickbutton, quickscan_button_image);
    gtk_widget_set_name(antiquickbutton,"home-quickscan");
    gtk_layout_put(GTK_LAYOUT(anti_layout), antiquickbutton, 326, 185);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%smyselfscan-image.png"),ANTIVIRUS_IMAGE_DIR);
    selfscan_above_image=gtk_image_new_from_file(image_dir);
    gtk_widget_show(selfscan_above_image);
    gtk_layout_put(GTK_LAYOUT(anti_layout), selfscan_above_image, 522, 65);
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%smyselfscan.png"),ANTIVIRUS_IMAGE_DIR);
    selfscan_button_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(selfscan_button_image);
    antimyselfbutton = gtk_button_new();
    gtk_button_set_relief((GtkButton *)antimyselfbutton,GTK_RELIEF_NONE);
    gtk_widget_set_size_request(antimyselfbutton, 138, 45);
    gtk_widget_show(antimyselfbutton);
   // gtk_button_set_image(antimyselfbutton,selfscan_button_image);
    gtk_widget_set_name(antimyselfbutton,"home-myselfscan");
    gtk_layout_put(GTK_LAYOUT(anti_layout), antimyselfbutton, 514, 185);

    char cmd[256];
    snprintf(cmd,sizeof(cmd),"%s%s_scan_time.txt",ANTIVIRUS_TEMP_DIR,g_current_user);
    if(access(cmd,F_OK)!=0){
	last_scan_label = gtk_label_new(_("NFS anti protect your system"));
    }
    else{
    /*char year[10],month[10],day[10];
    get_last_scan_time(year,month,day);
    last_scan_label = gtk_label_new("");
    sprintf(last_scan_time,_("you last scan is:%syear,%smonth,%sday"),year,month,day);
    gtk_label_set_text(last_scan_label,last_scan_time);*/
	char year[10],month[10],day[10];
    if(get_last_scan_time(year,month,day)){
        last_scan_label = gtk_label_new("");
        sprintf(last_scan_time,_("you last scan is:%syear,%smonth,%sday"),year,month,day);
        gtk_label_set_text(last_scan_label,last_scan_time);
        }
    else{
                printf("上次扫描时间位置\n");
                last_scan_label = gtk_label_new(_("NFS anti protect your system"));
        }
    }
	printf("111111111111111111 bbbbbbbbbbbbbbbb\n");
    change_font_style((GtkLabel *)last_scan_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");
    gtk_widget_show(last_scan_label);

    last_scan_button = gtk_button_new();
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sleft.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *left_image1 = gtk_image_new_from_file(image_dir);
    gtk_widget_show(left_image1);
    gtk_widget_set_size_request(last_scan_button, 20, 21);
    gtk_button_set_relief((GtkButton *)last_scan_button,GTK_RELIEF_NONE);
    //gtk_button_set_image(last_scan_button,left_image1);
    gtk_widget_show(last_scan_button);
    gtk_widget_set_name(last_scan_button,"home-left2");
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sright.png"),ANTIVIRUS_IMAGE_DIR);
    GtkWidget *right_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(right_image);
    GtkWidget *last_update_button = gtk_button_new();
    gtk_widget_set_size_request(last_update_button, 20, 21);
    gtk_button_set_relief((GtkButton *)last_update_button,GTK_RELIEF_NONE);
   // gtk_button_set_image(last_update_button,right_image);
    gtk_widget_show(last_update_button);
    gtk_widget_set_name(last_update_button,"home-right");
    //last_update_label = gtk_label_new(_("update none"));
    look_last_update_button = gtk_button_new();
    gtk_widget_set_name(look_last_update_button,"home-look-upgrade-button");
    gtk_button_set_relief((GtkButton *)look_last_update_button,GTK_RELIEF_NONE);
    gtk_widget_set_size_request(look_last_update_button, 81,24);
#if 0
    char avlUpgradeLog[256];
    snprintf(avlUpgradeLog,sizeof(avlUpgradeLog),"%savlUpgradeDate.txt",ANTIVIRUS_LOG_DIR);
    if(access(avlUpgradeLog,F_OK)!=0){
        last_update_label = gtk_label_new(_("update none"));
    }
    else{
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
	printf("22222222222222222 bbbbbbbbbbbbbbbb\n");
    change_font_style((GtkLabel *)last_update_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");
#endif
    gtk_widget_show(look_last_update_button);

    GtkWidget *last_scan_update_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(last_scan_update_box, 800, 20);
    gtk_widget_show(last_scan_update_box);
    gtk_box_pack_start(GTK_BOX (last_scan_update_box),last_scan_button,FALSE,FALSE,20);
    gtk_box_pack_start(GTK_BOX (last_scan_update_box),last_scan_label,FALSE,FALSE,5);
    gtk_box_pack_end(GTK_BOX (last_scan_update_box),last_update_button,FALSE,FALSE,10);
    gtk_box_pack_end(GTK_BOX (last_scan_update_box),look_last_update_button,FALSE,FALSE,10);
    gtk_layout_put(GTK_LAYOUT(anti_layout), last_scan_update_box, 0, 340);
  
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sleft.png"),ANTIVIRUS_IMAGE_DIR);
  
    GtkWidget *left_image2 = gtk_image_new_from_file(image_dir);
    gtk_widget_show(left_image2);
    GtkWidget *look_devide_button = gtk_button_new();
    gtk_widget_set_size_request(look_devide_button, 20,18);
    gtk_button_set_relief((GtkButton *)look_devide_button,GTK_RELIEF_NONE);
    gtk_button_set_image(look_devide_button,left_image2);
    gtk_widget_show(look_devide_button);
    gtk_widget_set_name(look_devide_button,"home-left1");
    look_devide_label = gtk_label_new(_("look over devide file"));
    /*change_font_style((GtkLabel *)look_devide_label,"<span foreground=\"#FFFFFF\" font_desc='10'>%s</span> ");
    gtk_widget_show(look_devide_label);*/
    look_devide_label_button = gtk_button_new();
    gtk_button_set_relief((GtkButton *)look_devide_label_button,GTK_RELIEF_NONE);
    gtk_widget_set_size_request(look_devide_label_button, 71,24);
//    gtk_container_add(look_devide_label_button,look_devide_label);
    gtk_widget_set_name(look_devide_label_button,"home-look-devide-button");
    gtk_widget_show(look_devide_label_button);
  //  g_signal_connect(G_OBJECT(look_devide_label_button),"enter",G_CALLBACK(enter_look_devide_button),look_devide_label_button);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sclick_start_defence.png"),ANTIVIRUS_IMAGE_DIR);
    click_start_defence_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(click_start_defence_image);
    click_start_defence_button=gtk_button_new();
    gtk_widget_set_size_request(click_start_defence_button, 123, 28);
    gtk_button_set_relief((GtkButton *)click_start_defence_button,GTK_RELIEF_NONE);
    //gtk_button_set_image(click_start_defence_button,click_start_defence_image);
    gtk_widget_set_name(click_start_defence_button,"start_defence");

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sclick_close_defence.png"),ANTIVIRUS_IMAGE_DIR);

    GtkWidget *click_close_defence_image = gtk_image_new_from_file(image_dir);
    gtk_widget_show(click_close_defence_image);
    click_close_defence_button=gtk_button_new();
    gtk_button_set_relief((GtkButton *)click_close_defence_button,GTK_RELIEF_NONE);
    gtk_widget_set_size_request(click_close_defence_button, 182, 28);
   // gtk_button_set_image(click_close_defence_button,click_close_defence_image);
    gtk_widget_set_name(click_close_defence_button,"close_defence");

    int defence_flag=get_defence_status();
    if (defence_flag==1)
    {

	gtk_widget_show(click_close_defence_button);
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sopen_defence.gif"),ANTIVIRUS_IMAGE_DIR);
	
        start_defence_image = gtk_image_new_from_file(image_dir);
    }
    else
    {
	gtk_widget_show(click_start_defence_button);
        memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sclose_defence.gif"),ANTIVIRUS_IMAGE_DIR);

        start_defence_image = gtk_image_new_from_file(image_dir);
    }
    gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(cssProvider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sclick_start_defence.png"),ANTIVIRUS_IMAGE_DIR);
    start_image = gtk_image_new_from_file(image_dir);
    
    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sclick_close_defence.png"),ANTIVIRUS_IMAGE_DIR);
    close_image = gtk_image_new_from_file(image_dir);


    GtkWidget *look_devide_click_start_defence_box  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(look_devide_click_start_defence_box, 800, 20);
    gtk_widget_show(look_devide_click_start_defence_box);
    gtk_box_pack_start(GTK_BOX (look_devide_click_start_defence_box),look_devide_button,FALSE,FALSE,20);
    gtk_box_pack_start(GTK_BOX (look_devide_click_start_defence_box),look_devide_label_button,FALSE,FALSE,0);
    gtk_box_pack_end(GTK_BOX (look_devide_click_start_defence_box),click_start_defence_button,FALSE,FALSE,40);
    gtk_box_pack_end(GTK_BOX (look_devide_click_start_defence_box),click_close_defence_button,FALSE,FALSE,40);
    gtk_box_pack_end(GTK_BOX (look_devide_click_start_defence_box),start_image,FALSE,FALSE,40);
    gtk_box_pack_end(GTK_BOX (look_devide_click_start_defence_box),close_image,FALSE,FALSE,40);
    gtk_layout_put(GTK_LAYOUT(anti_layout), look_devide_click_start_defence_box, 0, 310);

    memset(image_dir,0,sizeof(image_dir));
    snprintf(image_dir,sizeof(image_dir),_("%sopen_defence.gif"),ANTIVIRUS_IMAGE_DIR);
    start_defence_image = gtk_image_new_from_file(image_dir);
    gtk_layout_put(GTK_LAYOUT(anti_layout),start_defence_image, 660, 180); 
    g_signal_connect(G_OBJECT(look_devide_label_button),"clicked",G_CALLBACK(look_devide),NULL);
    g_signal_connect(G_OBJECT(click_start_defence_button),"clicked",G_CALLBACK(start_defence),start_defence_image);
   // g_signal_connect(G_OBJECT(click_start_defence_button),"enter",G_CALLBACK(enter_start_defence),click_start_defence_image);
    g_signal_connect(G_OBJECT(click_close_defence_button),"clicked",G_CALLBACK(close_defence),start_defence_image);
   // g_signal_connect(G_OBJECT(click_close_defence_button),"enter",G_CALLBACK(enter_close_defence),click_close_defence_image);
    g_signal_connect(G_OBJECT(last_update_button),"clicked",G_CALLBACK(show_or_hide_last_update),look_last_update_button);
    g_signal_connect(G_OBJECT(look_devide_button),"clicked",G_CALLBACK(show_or_hide_look_devide_button),look_devide_label_button);
    g_signal_connect(G_OBJECT(antiallscanbutton),"clicked",G_CALLBACK(all_scan_type),window);
    g_signal_connect(G_OBJECT(antiquickbutton),"clicked",G_CALLBACK(quick_scan_type),window);
    g_signal_connect(G_OBJECT(antimyselfbutton),"clicked",G_CALLBACK(myself_scan_type),window);
    g_signal_connect(G_OBJECT(last_scan_button),"clicked",G_CALLBACK(hide_last_scan_time),last_scan_label);
    g_signal_connect(G_OBJECT(look_last_update_button),"clicked",G_CALLBACK(look_update_time),NULL);
    gtk_box_pack_start(GTK_BOX (antivbox),anti_layout,FALSE,FALSE,0);
    gtk_grid_attach((GtkGrid *)table1,antivbox,0,0,1,1);

	printf("3333333333333333 bbbbbbbbbbbbbbbb\n");

}
//创建杀毒界面
void create_antivirus_page()
{
    //创建杀毒软件页面框架
    create_frame_page();

    //创建扫描按钮
    create_scan_button();
} 

//创建设置界面
void create_settings_page()
{
    table2 = gtk_grid_new();
    antivirus_settings_page(table2);
}

//创建日志界面
void create_log_page()
{
    table3 = gtk_grid_new();
    init_log_window(table3);
}

void show_home_page(char *info)
{
    gtk_container_add(GTK_CONTAINER(thingbox),table1);
    gtk_container_add(GTK_CONTAINER(thingbox),table2);
    gtk_container_add(GTK_CONTAINER(thingbox),table3);
    gtk_container_add(GTK_CONTAINER(thingbox),table4);
    gtk_container_add(GTK_CONTAINER(thingbox),table6);
    gtk_container_add(GTK_CONTAINER(thingbox),real_protect_result_table);
    gtk_container_add(GTK_CONTAINER(vbox),thingbox);
    area = 1;
    gtk_widget_show(table1);
    show_antivirus_version();

    if(strcmp(info,"not-autostart") == 0)
    {
        gtk_widget_show((GtkWidget *)window);
    }
}


//创建杀毒软件主界面
void create_home_page(char *info)
{

        //创建杀毒界面
        create_antivirus_page();

        //创建设置界面
        create_settings_page();

		printf("create settings \n");
        //创建扫描过程界面
        create_scan_process_page();

		printf("create scan page over \n");

	//创建日志界面
        create_log_page();
		printf("create log page over \n");


        //创建关于界面
        create_about_page();
	detail_result(0,0,0,"0","0");

	real_protect_result();

        //设置杀毒软件图标展示在托盘中
        set_icon_in_tray(info);

        //显示主界面
        show_home_page(info);
}

int refreshMainPageIntroNum;
gboolean refresh_main_page_introduce(gpointer user_data)
{
    char image_dir[256];
    if( mainPageIntroduceImage != NULL )
    {
       gtk_widget_destroy(mainPageIntroduceImage);
    }
    refreshMainPageIntroNum = (refreshMainPageIntroNum + 1) % 3;
    if( refreshMainPageIntroNum == 1)
    {
    
        snprintf(image_dir,sizeof(image_dir),_("%stiming_tile.png"),ANTIVIRUS_IMAGE_DIR);
        mainPageIntroduceImage = gtk_image_new_from_file(image_dir);
    }
    else if( refreshMainPageIntroNum == 2)
    {
	memset(image_dir,0,sizeof(image_dir));
        snprintf(image_dir,sizeof(image_dir),_("%sselfscan_title.png"),ANTIVIRUS_IMAGE_DIR);
        mainPageIntroduceImage = gtk_image_new_from_file(image_dir);
    }
    else
    {
       memset(image_dir,0,sizeof(image_dir));
       snprintf(image_dir,sizeof(image_dir),_("%sallscan_title.png"),ANTIVIRUS_IMAGE_DIR);
       mainPageIntroduceImage = gtk_image_new_from_file(image_dir);
    }
    gtk_widget_show(mainPageIntroduceImage);
    gtk_layout_put(GTK_LAYOUT(layout_main), mainPageIntroduceImage, 0, 17);

    return 1;
}


void * refreshMainPageIntroduce(void * arg)
{
    gint refreshMainPageId;
    refreshMainPageId = g_timeout_add (3*1000, refresh_main_page_introduce, NULL);
}

