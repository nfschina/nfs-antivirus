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
#ifndef HOME_HANDLE_H
#define HOME_HANDLE_H
void execution_scan(int scanType,char *scanPath); //执行扫描过程函数
int get_last_scan_time(char *year,char *month,char *day);
void show_last_scan_time(GtkWidget *widget,gpointer *data);
void hide_last_scan_time(GtkWidget *widget,gpointer *data);
void show_or_hide_look_devide_button(GtkWidget *widget,gpointer *data);
void show_or_hide_last_update(GtkWidget *widget,gpointer *data);
void start_defence(GtkWidget *widget,gpointer *data);
void look_devide(GtkWidget *widget,gpointer *data);
int  get_last_upgrade_time(char *year,char *month,char *day);
void look_update_time(GtkWidget *widget,gpointer *data);
void check_update(GtkWidget *widget,gpointer *data);
void feedback_func(GtkWidget *widget,gpointer *data);
void enter_min_button(GtkWidget *widget,gpointer *data);
void enter_antibutton(GtkWidget *widget,gpointer *data);
void enter_setbutton(GtkWidget *widget,gpointer *data);
void enter_quick_button(GtkWidget *widget,gpointer *data);
void enter_allscan(GtkWidget *widget,gpointer *data);
void enter_myselfscan(GtkWidget *widget,gpointer *data);
void enter_min_button(GtkWidget *widget,gpointer *data);
void enter_close_button(GtkWidget *widget,gpointer *data);
void close_defence(GtkWidget *widget,gpointer *data);
void enter_start_defence(GtkWidget *widget,gpointer *data);
void enter_close_defence(GtkWidget *widget,gpointer *data);
void enter_look_devide_button(GtkWidget *widget,gpointer *data);
void enter_checkupdate(GtkWidget *widget,gpointer *data);
void enter_feedback(GtkWidget *widget,gpointer *data);
void enter_feedback_white_bg(GtkWidget *widget,gpointer *data);
void enter_checkupdate_white_bg(GtkWidget *widget,gpointer *data);
void clamav_daemon_operator();
#endif
