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
#ifndef _SETTING_HANDLE_H
#define _SETTING_HANDLE_H
#define BOOT_START_TRUE  1
#define BOOT_START_FALSE  0
void save_scan_anti(GtkWidget *widget,gpointer data);
void save_common_settings(GtkWidget *widget,gpointer data);
void save_update_set(GtkWidget *widget,gpointer data);
void save_scan_style_set(GtkWidget *widget,gpointer data);
void remove_devide_file_list(GtkWidget *widget,gpointer data);
void get_devide_file_used_space(char *used_space);
int get_select_items_num(int is_new_path);
int write_white_list(char *logFileName ,char *white_file);
int select_devide_file(GtkWidget *widget,gpointer *data);
//void recovery_default(GtkWidget *widget,gpointer data);
void common_recovery_default(GtkWidget *widget,gpointer data);
void anti_scan_recovery_default(GtkWidget *widget,gpointer data);
void update_recovery_default(GtkWidget *widget,gpointer data);
void enter_add_button(GtkWidget *widget,gpointer data);
void recovery_default_enter(GtkWidget *widget,gpointer data);
void leave_savebutton(GtkWidget *widget,gpointer data);
void save_set_enter(GtkWidget *widget,gpointer data);
void leave_add_regular(GtkWidget *widget,gpointer data);
void enter_add_regular(GtkWidget *widget,gpointer data);
void leave_add_file(GtkWidget *widget,gpointer data);
void enter_add_file(GtkWidget *widget,gpointer data);
void leave_add_path(GtkWidget *widget,gpointer data);
void enter_add_path(GtkWidget *widget,gpointer data);
void leave_remove_button(GtkWidget *widget,gpointer data);
void enter_remove_button(GtkWidget *widget,gpointer data);
void leave_recovery_button(GtkWidget *widget,gpointer data);
void enter_recovery_button(GtkWidget *widget,gpointer data);
int select_protect_dir(GtkWidget *widget,gpointer *data);
int check_start_real_protect();
#endif
