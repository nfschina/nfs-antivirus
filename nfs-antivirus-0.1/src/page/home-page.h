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
#define _(STRING) gettext(STRING)
void create_home_page(char *info);
gint  closeall(GtkWidget *widget,gpointer data); //询问是否退出杀毒软件
void antiwindow(GtkWidget *widget,gpointer data); //显示杀毒界面
void setwindow(GtkWidget *widget,gpointer data); //显示设置界面
void show_antivirus_version(); //显示杀毒软件版本号
/**扫描指定文件*/
void specified_scan_type(char *scanPath,int scanType);
void * refreshMainPageIntroduce(void * arg);
gint button_press_event(GtkWidget * widget,GdkEventButton * event, gpointer data);
gint button_release_event(GtkWidget * widget, GdkEventButton * event,gpointer data);
gint motion_notify_event(GtkWidget * widget, GdkEventButton * event,gpointer data);         // 鼠标移动事件
